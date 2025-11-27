#include <QAccessible>
#include <QLabel>
#include <QThread>
#include <algorithm> // используем std::find, std::distance
#include <QDesktopServices>

#include "utilities.h"
#include "topwindow.h"
#include "ui_topwindow.h"
#include "LoggerMacros.h"

// ============================================================================
// TopWindow: основной класс приложения
// ----------------------------------------------------------------------------
// Здесь содержится логика загрузки плагинов, взаимодействия с окном менеджера,
// обработки ввода, озвучивания (NVDA) и удаления плагинов.
// Важно: в комментариях подчёркнуты места, которые часто приводят к крашам:
//  - работа с QPluginLoader::instance() и unload()
//  - порядок удаления/освобождения объектов/указателей
//  - синхронизация UI (QListWidget) и внутренних векторов
// ============================================================================

TopWindow::TopWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TopWindow)
{
    ui->setupUi(this);
    Utilities::clearTombstone("plugins"); //Утилита удаления мусора

    // --- Доступность ---
    // Назначаем "имя" для поля ввода, чтобы скринридеры могли идентифицировать элемент.
    ui->enterText->setAccessibleName("Поле ввода");

    // Загружаем справочный текст из resource JSON (путь :/reference.json)
    // Функция loadReferenceFromJson возвращает QString справки или пустую строку.
    reference = loadReferenceFromJson();

    // Создаём окно-менеджер плагинов. std::unique_ptr используется для автоматического управления.
    mw = std::make_unique<Manager>(this);
    sm = std::make_unique<StateManager>(this);

    // Настройка внешнего вида основного окна
    setWindowTitle("AGL-Player");
    setMinimumSize(781, 491);

    // Устанавливаем фильтр событий для headerText — потом в eventFilter используется своя логика.
    ui->headerText->installEventFilter(this);

    // Создаём и настраиваем действия меню/горячие клавиши
    createActionsName();

    // Загружаем плагины. Если не удалось — показываем информацию (не фатальная ошибка).
    // loadPlugin() наполняет mw->namePlugin, filePaths, pluginsLoad, pluginsInterface.
    if(!loadPlugin())
    {
        QMessageBox::information(this, tr("Ошибка!"), tr("Не удалось загрузить плагин!"));
    }
    // Обновляем UI списка плагинов в менеджере. Manager::updateLists добавляет элементы в QListWidget.
    mw->updateLists();

    // Открываем менеджер (показываем его и ставим фокус)
    managerOpen();

    // Подключаем сигнал itemActivated только если список не пуст — это снижает шанс лишних вызовов.
    QListWidget* pluginList = mw->getPlugList();
    if(pluginList->count() != 0) {
        connect(pluginList, &QListWidget::itemActivated, this, &TopWindow::onPlugSelected);
    }

    // Подписываемся на глобальную смену фокуса, чтобы управлять озвучкой NVDA:
    // - при попадании фокуса на headerText читаем его текст
    // - при уходе — останавливаем озвучку
    connect(qApp, &QApplication::focusChanged, this, [this](QWidget* oldWidget, QWidget* newWidget)
            {
                if(oldWidget == ui->headerText && newWidget == ui->enterText) talkNVDA_.stopSpeak();
                if(newWidget == ui->headerText) talkNVDA_.speakTextNVDA(ui->headerText->toPlainText());
            }); //Здесь ловим смену фокуса через QApplication::focusChanged, чтобы управлять озвучкой

    // Подписываемся на сигнал удаления плагина от окна менеджера.
    // В Manager сигнал defined как: void deletePlugin(QListWidgetItem*);
    connect(mw.get(), &Manager::deletePlugin, this, &TopWindow::deletePlug);
    connect(mw.get(), &Manager::update, this, &TopWindow::updatePlug);
    connect(ui->startGameAgain, &QAction::triggered, this, &TopWindow::againGame);
    connect(ui->saveGame, &QAction::triggered, this, &TopWindow::saveGame);
    connect(mw.get(), &Manager::loadGame, this, &TopWindow::loadLastGame);
    connect(ui->openSaveDontReload, &QAction::triggered, this, &TopWindow::fastLoad);
    connect(ui->undo, &QAction::triggered, this, &TopWindow::undo);
    connect(ui->redo, &QAction::triggered, this, &TopWindow::redo);
    //открытие лог файла игры
    connect(ui->gameLogFile, &QAction::triggered, this,&TopWindow::openGameLog);
}

TopWindow::~TopWindow()
{
    // Удаляем UI. Остальные ресурсы — либо std::unique_ptr, либо векторы которые мы очищаем отдельно.
    delete ui;
}

// ----------------------------
// Слот: пользователь активировал элемент (двоичный клик/Enter) в списке плагинов
// ----------------------------
void TopWindow::onPlugSelected(QListWidgetItem* item)
{
    LOG_FUNC_START();
    currentItem = item;
    // Если элемент пустой — ничего не делаем (защита от nullptr)
    if(!item)
    {
        LOG_WARN(QString("Невалидный указатель QListWidgetItem!"));
        return;
    }
    QString plugName = item->text();

    // Ищем соответствие имени в нашем списке загруженных плагинов
    // (mw->namePlugin должен быть в том же порядке, что и pluginsLoad/pluginsInterface/filePaths).
    for(int i = 0; i < mw->namePlugin.size(); ++i)
    {
        if(mw->namePlugin[i] == plugName)
        {
            // Берём интерфейс для плагина — это указатель, полученный при loadPlugin().
            gameInterface = pluginsInterface[i];

            if(gameInterface)
            {
                //Чистим StateManager
                sm->clearState();
                //Начинаем логировать игру
                START_GAME_LOG(plugName);
                // Отображаем стартовое сообщение плагина в headerText
                LOG_PC_ANSWER(gameInterface->startMessage(), plugName);
                announceSetText(ui->headerText, gameInterface->startMessage());

                // Переподключаем ввод: убираем старый слот (если был) и подключаем runGame.
                // Это нужно, чтобы ввод Enter отправлял данные в текущий плагин.
                disconnect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::runGame);
                connect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::runGame);
                LOG_INFO(QString("Ввод переподключен к runGame"));
                LOG_FUNC_END(QString("Плагин " + plugName + " успешно открыт!"));
                return;
            }
            else
            {
                // Если по какой-то причине плагин не реализует GameInterface — предупреждаем пользователя.
                QMessageBox::warning(this, "Ошибка", "Плагин не реализует GameInterface");
                LOG_ERR(QString("Плагин " + plugName + " не реализует GameInterface"));
                return;
            }
        }
    }

    LOG_WARN(QString("Плагин " + plugName + " не найден среди загруженных"));
}

// ============================================================================
// deletePlug — удаление плагина (вызывается из Manager)
// ----------------------------------------------------------------------------
// Подсказка: здесь много "подводных камней":
//  - порядок действий: сначала убрать все ссылки/сигналы на instance, затем unload(), затем удалить файл.
//  - QPluginLoader::unload() удалит instance (если он родитель для instance); но если где-то остались живые объекты
//    с кодом из плагина — unload() возвратит false и DLL останется заблокированной.
//  - нужно поддерживать согласованность между QListWidget (UI) и внутренними векторами (имена/путь/загрузчики).
// ============================================================================
void TopWindow::deletePlug(QListWidgetItem* item)
{
    LOG_FUNC_START();
    // ---------- 0) Базовые проверки ----------
    if(item == nullptr)
    {
        LOG_WARN(QString("Невалидный указатель QListWidgetItem!"));
        return; // защита от некорректного вызова
    }

    // Ищем индекс плагина по имени в mw->namePlugin.
    // Важно: используем имя (item->text()), а не указатели, потому что мы синхронизируем векторы по индексам.
    auto iter = std::find(mw->namePlugin.begin(), mw->namePlugin.end(), item->text());
    if(iter == mw->namePlugin.end())
    {
        LOG_ERR(QString("Плагин не найден!"));
        return; // если не нашли — выходим (может уже удалён)
    }

    QString plugName = *iter;
    int index = std::distance(mw->namePlugin.begin(), iter);

    // Делаем дополнительную защиту: index должен быть валиден для filePaths/other vectors.
    if (index < 0 || static_cast<size_t>(index) >= filePaths.size()) {
        QMessageBox::warning(this, tr("Error!"), tr("Плагин не найден!"));
        LOG_ERR(QString("Индекс элемента невалиден!"));
        return;
    }

    // Получаем QPluginLoader* для данного индекса
    QPluginLoader* loader = pluginsLoad[index];
    if (!loader) {
        QMessageBox::warning(this, tr("Error!"), tr("Загрузчик плагина отсутствует!"));
        LOG_ERR(QString("Загрузчик плагина отсутствует!"));
        return;
    }

    // Получаем путь (строка) к файлу плагина — для логов и удаления.
    const QString path = filePaths[index];

    // ---------- 1) Аккуратно «отцепляемся» от instance ----------
    // Если instance() != nullptr, значит сейчас в памяти есть объект плагина.
    // Наша цель: гарантировать, что **никто** больше не держит ссылки/сигналы на объекты плагина,
    // потому что unload() удалит/освободит модуль, и обращение к таким объектам приведёт к крашу.
    QObject* instance = loader->instance(); // может быть nullptr, если уже выгружен

    if (instance) {
        // Отключаем все соединения связанных сигналов/слотов у instance
        // Это эквивалент instance->disconnect(nullptr, nullptr, nullptr, nullptr)
        // и защищает от случаев, когда слоты в основном приложении ещё подписаны на сигналы плагина.
        instance->disconnect();

        // Если мы в UI держали указатель на интерфейс (gameInterface), обнуляем его,
        // чтобы в дальнейшем не обращаться к освобождённому объекту.
        if (gameInterface == pluginsInterface[index])
        {
            gameInterface = nullptr;
        }

        // В нашем internal векторе мы тоже сбрасываем указатель, чтобы не использовать его после unload.
        pluginsInterface[index] = nullptr;

        // ВАЖНО: не делаем delete(instance) или instance->deleteLater()!
        // По документации QPluginLoader::unload() сам удалит instance синхронно при успешном unload().
        // Если вы вызовете delete вручную, может случиться двойное удаление.
        // Поэтому здесь только отключаем связи и обнуляем все внешние указатели.
    }

    // ---------- 2) Выгружаем .dll через QPluginLoader ----------
    // QPluginLoader::unload() синхронно попытается выгрузить модуль и уничтожить instance.
    // Если какие-то объекты плагина живы (включая дочерние объекты, таймеры, запущенные потоки,
    // открытые файловые дескрипторы и т.п.), unload вернёт false.
    if (!loader->unload()) {
        LOG_ERR(QString("[Plugin] unload FAILED for" + path
                   + "error:" + loader->errorString()));

        // Сообщаем пользователю — вот типичная причина, почему файл нельзя удалить:
        // остались живые объекты, таймеры или другой QPluginLoader держит библиотеку открытой.
        QMessageBox::warning(this, tr("Error!"),
                             tr("Не удалось выгрузить плагин (остались живые объекты)."));
        LOG_ERR(QString("Не удалось выгрузить плагин (остались живые объекты)."));
        return;
    }

    // Иногда Windows некоторое время удерживает дескриптор. Чтобы повысить шанс удаления,
    // даём событийному циклу поработать (небольшая "передышка").
    for (int i = 0; i < 3; ++i) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
    }

    // ---------- 3) Пробуем удалить файл ----------
    // После успешного unload() файл обычно становится доступен для удаления.
    // Но возможны внешние факторы: антивирус, индексатор, другой процесс и т. д.
    bool removed = false;
    for (int attempt = 0; attempt < 5 && !(removed = QFile::remove(path)); ++attempt) {
        LOG_WARN(QString("[Plugin] remove failed, retry for" + path));
        QThread::msleep(30); // даём немного времени между попытками
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
    }

    if (!removed) {
        // Если remove не прошёл, часто помогает переименование — тогда по крайней мере UI знает, что плагин "удален".
        const QString tombstone = path + ".deleted." + QString::number(QDateTime::currentMSecsSinceEpoch());
        if (QFile::rename(path, tombstone))
        {
            // rename успешен — считайте, что физически "удалили" плагин (переименовали).
            LOG_INFO(QString("[Plugin] remove failed, but rename to tombstone succeeded:" + tombstone));
        }
        else
        {
            // Если ни удалить, ни переименовать нельзя — сообщаем пользователю о причинах.
            QMessageBox::warning(this, tr("Error!"),
                                 tr("Не удалось удалить/переименовать файл плагина.\n"
                                    "Возможные причины: антивирус/индексатор блокирует файл, другой процесс держит DLL."));
            LOG_ERR(QString("Не удалось удалить/переименовать файл плагина.\n"
                            "Возможные причины: антивирус/индексатор блокирует файл, другой процесс держит DLL."));

            // Пытаемся восстановить состояние: reload() чтобы снова иметь рабочий instance.
            if (!loader->load())
            {
                LOG_ERR(QString("[Plugin] reload also failed for" + path + "error:" + loader->errorString()));
            }
            else
            {
                QObject* inst2 = loader->instance();
                GameInterface* iface2 = inst2 ? qobject_cast<GameInterface*>(inst2) : nullptr;
                if(iface2)
                {
                    pluginsInterface[index] = iface2; // восстанавливаем указатель, если удалось загрузить
                    LOG_FUNC_END(QString("Плагин " + plugName + " успешно востановлен!"));
                    return;
                }
            }
            LOG_FUNC_END(QString("Ошибка восстановления плагина " + plugName + " GameInterface = nullptr"));
            return;
        }
    }

    // ---------- 4) Всё успешно — чистим контейнеры/интерфейсы ----------
    // QPluginLoader уже выгрузил библиотеку (unload), мы можем удалить объект загрузчика.
    delete loader;

    // Удаляем записи о плагине из внутренних структур в строго согласованном порядке:
    // namePlugin (UI список имен), pluginsInterface, pluginsLoad, filePaths.
    // Важно: порядок одинаковый во всех векторах — index указывает на один и тот же плагин.
    mw->namePlugin.removeAt(index);
    pluginsInterface.removeAt(index);
    pluginsLoad.removeAt(index);
    filePaths.removeAt(index);

    // Удаляем элемент QListWidget (строку) — если был передан.
    if (item)
    {
        delete item; // безопасно: item удаляем, он убирается из QListWidget
    }

    QMessageBox::information(this, tr("Success!"), tr("Плагин удалён."));
    LOG_FUNC_END(QString("Плагин " + plugName + " успешно удалён!"));
}

void TopWindow::updatePlug()
{

    //TODO реализовать корректное удаление плагинов из системы, либо сделать буфер для сверки обновлений
    LOG_FUNC_START();
    // ---------- 0) Сохраняем текущий размер списка плагинов ----------
    // Нужен для того, чтобы после загрузки новых плагинов посчитать, сколько добавилось
    int currentPlugSize = mw->namePlugin.size();
    QVector<QString> oldNames(mw->namePlugin);
    Utilities::clearItems(mw->getPlugList(), mw->getPlugList2(), oldNames);

    // ---------- 1) Пытаемся загрузить плагины ----------
    // Если loadPlugin() вернет false, значит загрузка не удалась
    if(!loadPlugin())
    {
        // Информируем пользователя о проблеме
        QMessageBox::information(this, tr("Ошибка!"), tr("Не удалось загрузить плагин!"));
        LOG_ERR(QString("Не удалось загрузить плагин!"));
        return; // Выходим из слота, дальше делать нечего
    }

    // ---------- 2) Получаем новый размер списка после апдейта ----------
    int updatePlugSize = mw->namePlugin.size();

    // ---------- 3) Вычисляем, сколько новых плагинов добавлено ----------
    int resultUpdate = updatePlugSize - currentPlugSize;

    // ---------- 4) Формируем сообщение для пользователя ----------
    // Например: "Добавлено 3 игр!"
    QString result = "Добавлено " + QString::number(resultUpdate) + " игр!";

    //----------- 5) Обновляем вектор с именами плагинов, для отображения их в менеджере ----------
    mw->updateLists();

    // ---------- 6) Показываем информационное окно ----------
    // tr(result) не нужен, так как result уже QString, достаточно передать его напрямую
    QMessageBox::information(this, tr("Info!"), result);
    LOG_FUNC_END(result);
}

void TopWindow::againGame()
{
    LOG_FUNC_START();
    // ---------- 0) Сброс текущей игры ----------
    // Обнуляем указатель на текущий игровой интерфейс,
    // чтобы старый плагин больше не обрабатывал ввод
    gameInterface = nullptr;
    sm->clearState();

    // ---------- 1) Отключаем обработку Enter в поле ввода ----------
    // Если пользователь нажимает Enter, слот runGame больше не будет вызываться для старой игры
    disconnect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::runGame);

    // ---------- 2) Перезапуск игры ----------
    // Повторно выбираем текущий плагин из списка, чтобы инициализировать его заново
    // onPlugSelected установит gameInterface, подключит слот runGame и обновит headerText
    onPlugSelected(currentItem);
    START_GAME_LOG(currentItem->text());
    LOG_FUNC_END(QString("Новая игра " + currentItem->text() + " начата!"));
}

// ============================================================================
// Перезагрузка списка плагинов: scan plugins folder и загружаем все рабочие плагины
// ----------------------------------------------------------------------------
// loadPlugin() очищает старое состояние и наполняет внутренние вектора заново.
// Обратите внимание на порядок: mw->namePlugin, filePaths, pluginsLoad, pluginsInterface.
// ============================================================================
bool TopWindow::loadPlugin()
{
    LOG_FUNC_START();
    // ---------- Очистка старого состояния ----------
    // mw->namePlugin: список отображаемых имён — Manager::updateLists читает именно его.
    mw->namePlugin.clear();

    // pluginsInterface: вектор сырых указателей на интерфейс (не владеем объектом)
    pluginsInterface.clear();

    // DeleteAll(pluginsLoad): пользовательская утилита, которая delete'ит все QPluginLoader* и очищает вектор.
    // Это важно: если не удалить старые QPluginLoader-ы, они будут держать DLL в памяти.
    // Utilities::DeleteAll<QPluginLoader>(pluginsLoad);
    for(auto& loader : pluginsLoad) {
        if(loader->instance()) loader->instance()->disconnect();
        loader->unload();
        delete loader;
    }

    // filePaths: пути к каждому плагину
    filePaths.clear();

    // Переходим в папку plugins (относительно applicationDirPath)
    QDir pluginsDir(QCoreApplication::applicationDirPath());
    if (!pluginsDir.cd("plugins"))
    {
        LOG_ERR(QString("Ошибка входа в директорию plugins!"));
        return false;
    }

    // Перечисляем файлы в каталоге (QDir::Files)
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString& fileName : entries)
    {
#ifdef Q_OS_WIN
        // Фильтруем по расширению DLL на Windows
        if(!fileName.endsWith(".dll", Qt::CaseInsensitive)) { continue; }
#else
        // Логика для Linux/macOS при необходимости: .so / .dylib
#endif

        const QString fullPath = pluginsDir.absoluteFilePath(fileName);

        // Создаём QPluginLoader без parent (мы сами будем управлять временем жизни loader)
        QPluginLoader* loader = new QPluginLoader(fullPath);

        // instance() попытается загрузить плагин и вернуть объект-инстанс.
        // Если плагин не загрузился — instance == nullptr.
        QObject* instance = loader->instance();

        if (!instance) {
            // Сообщаем в лог почему не удалось загрузить конкретный файл.
            LOG_ERR(QString("[Plugin] load failed for" + fullPath + "error:" + loader->errorString()));
            delete loader;
            continue;
        }

        // Читаем MetaData для отображаемого имени плагина (опционально).
        QJsonObject meta = loader->metaData().value("MetaData").toObject();
        QString displayName = meta.value("name").toString();
        if(displayName.isEmpty()) { displayName = fileName; }

        // Пробуем привести instance к нашему интерфейсу GameInterface
        GameInterface* iface = qobject_cast<GameInterface*>(instance);
        if (!iface) {
            // Плагин не реализует требуемый интерфейс — выгружаем и освобождаем loader.
            LOG_ERR(QString("[Plugin] doesn't implement GameInterface:" + fullPath));
            loader->unload(); // пытаемся корректно выгрузить
            delete loader;
            continue;
        }

        // Всё успешно — сохраняем информацию о плагине.
        // ВАЖНО: порядок векторов должен совпадать — index указывает на одну и ту же сущность.
        mw->namePlugin.push_back(displayName);
        filePaths.push_back(fullPath);
        pluginsLoad.push_back(loader);
        pluginsInterface.push_back(iface);
        LOG_INFO(QString("Плагин " + displayName + " успешно загружен!"));
    }

    return !pluginsLoad.isEmpty();
}

// ============================================================================
// createActionsName: настройка действий (QAction), хоткеев и подсказок
// ----------------------------------------------------------------------------
// Здесь setShortcut/setStatusTip + некоторые connect'ы. Комментарии лишь про подсказки
// и что нужно держать в уме при изменении хоткеев.
// ============================================================================
void TopWindow::createActionsName()
{
    ui->gameManager->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    ui->gameManager->setStatusTip(tr("Открыть менеджер игр"));
    connect(ui->gameManager, &QAction::triggered, this, &TopWindow::managerOpen);

    ui->addGame->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    ui->addGame->setStatusTip(tr("Добавить игру в библиотеку"));

    ui->openSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    ui->openSave->setStatusTip(tr("Открыть сохранение"));

    ui->openSaveDontReload->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_O));
    ui->openSaveDontReload->setStatusTip(tr("Открыть сохранение без перезагрузки библиотеки"));

    ui->saveGame->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    ui->saveGame->setStatusTip(tr("Сохранить игру"));

    ui->gameLogFile->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    ui->gameLogFile->setStatusTip(tr("Открыть файл игрового лога"));

    ui->startGameAgain->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    ui->startGameAgain->setStatusTip(tr("Начать игру заново"));

    ui->undo->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    ui->undo->setStatusTip(tr("Отменить последнее действие"));

    ui->redo->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_W));
    ui->redo->setStatusTip(tr("Вернуть последнее действие"));

    ui->resetAllSettings->setStatusTip(tr("Сбросить все настройки"));
    ui->resetAllSettings->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Z);

    ui->quit->setStatusTip(tr("Выход"));
    ui->quit->setShortcut(QKeySequence::Close);
    connect(ui->quit, &QAction::triggered, this, &TopWindow::exit);


    ui->setting->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_T));
    ui->setting->setStatusTip("Настройка");


    ui->search->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
    ui->search->setStatusTip(tr("Поиск"));

    ui->searchFurther->setShortcut(QKeySequence(Qt::Key_F1));
    ui->searchFurther->setStatusTip(tr("Найти далее"));

    ui->sceneText->setShortcut(QKeySequence(Qt::Key_F2));
    ui->sceneText->setStatusTip(tr("Текст сцены"));

    ui->objectList->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_1));
    ui->objectList->setStatusTip(tr("Перейти к списку объектов"));

    ui->inventList->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_2));
    ui->inventList->setStatusTip(tr("Перейти к списку инвентаря"));

    ui->wayList->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_3));
    ui->wayList->setStatusTip(tr("Перейти к списку путей"));

    ui->debugMode->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    ui->debugMode->setStatusTip(tr("Режим отладки"));

    ui->logOn->setShortcut(QKeySequence::Close);
    ui->logOn->setStatusTip(tr("Включить логирование"));


    ui->musicUp->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F1));
    ui->musicUp->setStatusTip(tr("Музыка громче"));

    ui->musicLow->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F2));
    ui->musicLow->setStatusTip(tr("Музыка тише"));

    ui->switchMusic->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F3));
    ui->switchMusic->setStatusTip(tr("Музыка включена"));

    ui->soundUp->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F1));
    ui->soundUp->setStatusTip(tr("Звуки громче"));

    ui->soundLow->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F2));
    ui->soundLow->setStatusTip(tr("Звуки Тише"));

    ui->switchSound->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F3));
    ui->switchSound->setStatusTip(tr("Звуки включены"));

    ui->alertUp->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_5));
    ui->alertUp->setStatusTip(tr("Уведомления громче"));

    ui->alertLow->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_6));
    ui->alertLow->setStatusTip(tr("Уведомления тише"));

    ui->switchAlert->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_7));
    ui->switchAlert->setStatusTip(tr("Уведомления включены"));

    ui->findOutput->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_M));
    ui->findOutput->setStatusTip(tr("Текущий способ вывода"));
}

// ============================================================================
// Утилита: вывод текста в виджет и установка фокуса
// ----------------------------------------------------------------------------
// qobject_cast<QPlainTextEdit*> безопасно проверяет тип.
// ============================================================================
void TopWindow::announceSetText(QWidget *widget, const QString &text)
{
    LOG_FUNC_START();
    // ---------- 0) Проверяем, что виджет является QPlainTextEdit ----------
    if (auto* plain = qobject_cast<QPlainTextEdit*>(widget))
    {
        // ---------- 1) Устанавливаем текст в виджет ----------
        plain->setPlainText(text);

        // ---------- 2) Делаем виджет активным ----------
        plain->setFocus();

        // ---------- 3) Озвучивание текста ----------
        if (talk_.currentReader() == "")
        {
            // Если текущий скринридер не определён, используем запасной NVDA
            talkNVDA_.speakWithFallback(ui->headerText, text);
            LOG_FUNC_END(QString("Используем NVDA!"));
        }
        else
        {
            // Иначе выводим текст через JAWS/Narrator
            talk_.output(text, true);
            LOG_FUNC_END(QString("Используем talk.dll!"));
        }
    }
}

// ============================================================================
// Загрузка справки из ресурса reference.json
// ----------------------------------------------------------------------------
// Формат ожидается: { "reference": "текст" }
// ============================================================================
QString TopWindow::loadReferenceFromJson()
{
    QFile file(":/reference.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        LOG_ERR(QString("Не удалось открыть reference.json"));
        return {};
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if(!doc.isObject())
    {
        LOG_ERR(QString("reference.json не содержит JSON-Object"));
        return {};
    }

    QJsonObject obj = doc.object();
    LOG_FUNC_END(QString("json файл успешно выгружен!"));
    return obj.value("reference").toString();
}

// ============================================================================
// Открытие окна менеджера плагинов
// ----------------------------------------------------------------------------
// Тут важно: manager окно modal/non-modal, фокус ставим асинхронно (singleShot)
// чтобы избежать двойного озвучивания/двойного выделения.
// ============================================================================
void TopWindow::managerOpen()
{
    LOG_FUNC_START();
    // Останавливаем озвучку, чистим поле ввода
    talkNVDA_.stopSpeak();
    ui->enterText->clear();
    gameInterface = nullptr;
    currentItem = nullptr;
    disconnect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::runGame);
    ui->headerText->setPlainText(reference);

    // При закрытии менеджера возвращаем справочный текст в headerText
    connect(mw.get(), &Manager::closeManagerWindow, this, [this](){
        announceSetText(ui->headerText, reference);
    });

    // Отвязываем отправку Enter'ом, потому что менеджер будет обрабатывать некоторые нажатия
    disconnect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::runGame);

    // Показываем менеджер и ставим фокус
    mw->show();
    mw->setFocus();

    // Через 300 мс ставим фокус на список и выделяем первую строку (если есть).
    // Причина: сразу после show() список может не получить фокус корректно, поэтому используем singleShot.
    QTimer::singleShot(300, [=]()
                       {
                           mw->getPlugList()->setFocus();
                           if(mw->getPlugList()->count() != 0) { mw->getPlugList()->setCurrentRow(0); }
                       }); // асинхронно ставим фокус, для выделения первой строки списка и убираем этим двойное проговаривание менеджер игр окно

    // Озвучиваем первое имя плагина — защищаемся проверкой isEmpty
    if(!mw->namePlugin.isEmpty())
    {
        talkNVDA_.speakTextNVDA(mw->namePlugin[0]);
    }
    LOG_FUNC_END(QString("Менеджер успешно открыт!"));
}

// ============================================================================
// Выход из приложения
// ============================================================================
void TopWindow::exit()
{
    QApplication::quit();
}

// ============================================================================
// Отправка текста в плагин (нажатие Enter в поле ввода)
// ----------------------------------------------------------------------------
// runGame использует gameInterface, поэтому нужно убедиться, что он установлен.
// ============================================================================
void TopWindow::runGame()
{
    LOG_FUNC_START();
    if(!gameInterface)
    {
        LOG_ERR(QString("Интерфейс невалиден!"));
        return;
    }
    if(ui->headerText->toPlainText() != "")
    {
        auto check = sm->snapshot(gameInterface, currentItem, ui->headerText);
        if(!check)
        {
            LOG_ERR(QString("StateManager->snapshot недоступен"));
        }
    }
    auto gameName = currentItem->text();
    // Печатаем введённый текст (в header или в enterText — announceSetText использует QPlainTextEdit)
    auto txt = ui->enterText->text();
    announceSetText(ui->enterText, txt);
    LOG_PLAYER_CHOICE(txt, gameName);

    // Получаем ответ от плагина; gameInput реализует логику игры
    QString text = gameInterface->gameInput(ui->enterText->text());
    ui->enterText->clear();

    // Если игра закончена — отключаем обработчик ввода и возвращаем справку
    if(gameInterface->isOver())
    {
        QMessageBox::information(this, "End game", tr("Спасибо за игру!"));
        ui->enterText->clear();
        gameInterface = nullptr;
        announceSetText(ui->headerText, reference);
        disconnect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::runGame);
        LOG_END_GAME(currentItem->text());
        currentItem = nullptr;
        LOG_FUNC_END(QString("Игра успешно завершена!"));
        return;
    }

    // Иначе отображаем ответ игры
    LOG_PC_ANSWER(text, gameName);
    announceSetText(ui->headerText, text);
}

void TopWindow::saveGame()
{
    LOG_FUNC_START();
    bool check = sm->save(gameInterface, currentItem, ui->headerText);

    if(!check)
    {
        LOG_ERR(QString("Не удалось сохранить " + currentItem->text()));
        return;
    }

    LOG_FUNC_END(QString("Плагин " + currentItem->text() + " успешно сохранен!"));
}

void TopWindow::loadLastGame(const QString& name)
{
    LOG_FUNC_START();
    // Ищем соответствие имени в нашем списке загруженных плагинов
    // (mw->namePlugin должен быть в том же порядке, что и pluginsLoad/pluginsInterface/filePaths).
    for(int i = 0; i < mw->namePlugin.size(); ++i)
    {
        if(mw->namePlugin[i] == name)
        {
            // Берём интерфейс для плагина — это указатель, полученный при loadPlugin().
            gameInterface = pluginsInterface[i];
            currentItem = mw->getPlugList()->item(i);
            if(!currentItem)
            {
                LOG_ERR(QString("currentItem = nullptr"));
                QMessageBox::warning(this, tr("Error"), tr("Ошибка загрузки игры!"));
                return;
            }

            if(gameInterface)
            {
                auto content = sm->load(gameInterface, currentItem);
                if(!content)
                {
                    LOG_ERR(QString("Ошибка загрузки сохранения!" + name));
                    QMessageBox::warning(this, tr("Error!"), tr("Ошибка загрузки сохранения!"));
                    return;
                }

                announceSetText(ui->headerText, content.value());

                disconnect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::runGame);
                connect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::runGame);

                LOG_INFO(QString("Ввод переподключен к runGame"));
                LOG_FUNC_END(QString("Плагин " + name + " успешно загружен!"));
                return;
            }
            else
            {
                // Если по какой-то причине плагин не реализует GameInterface — предупреждаем пользователя.
                QMessageBox::warning(this, "Ошибка", "Плагин не реализует GameInterface");
                LOG_ERR(QString("Плагин " + name + " не реализует GameInterface"));
                return;
            }
        }
    }
}

void TopWindow::fastLoad()
{
    if(!currentItem) return;
    mw->loadGame(currentItem->text());
}

void TopWindow::undo()
{
    LOG_FUNC_START();
    auto undoResult = sm->undo(gameInterface, currentItem, ui->headerText);
    if(!undoResult)
    {
        QMessageBox::warning(this, "Предупреждение", "Ошибка возврата предыдущего действия!");
        LOG_ERR(QString("Ошибка отмены предыдущего действия на уровне StateManager"));
        return;
    }

    auto result = gameInterface->loadState(undoResult.value());
    if(!result)
    {
        QMessageBox::warning(this, "Предупреждение", "Ошибка возврата предыдущего действия!");
        LOG_ERR(QString("Ошибка отмены предыдущего действия на уровне plugin " + currentItem->text()));
        return;
    }

    announceSetText(ui->headerText, result.value());
    LOG_FUNC_END(QString("undo успешно!"));
}

void TopWindow::redo()
{
    LOG_FUNC_START();
    auto redoResult = sm->redo(gameInterface, currentItem, ui->headerText);
    if(!redoResult)
    {
        QMessageBox::warning(this, "Предупреждение", "Ошибка возврата предыдущего действия!");
        LOG_ERR(QString("Ошибка возврата предыдущего действия на уровне StateManager"));
        return;
    }

    auto result = gameInterface->loadState(redoResult.value());
    if(!result)
    {
        QMessageBox::warning(this, "Предупреждение", "Ошибка возврата предыдущего действия!");
        LOG_ERR(QString("Ошибка отмены предыдущего действия на уровне plugin " + currentItem->text()));
        return;
    }

    announceSetText(ui->headerText, result.value());
    LOG_FUNC_END(QString("redo успешно"));
}

void TopWindow::openGameLog()
{
    if(!gameInterface)
    {
        QMessageBox::warning(this, "Предупрждение", "Чтобы открыть лог файл, сначала откройте игру!");
        LOG_WARN(QString("Игра не выбрана!"));
        return;
    }

    auto path = Logger::instance().getPath(currentItem->text());
    if(!path)
    {
        QMessageBox::warning(this, "Ошибка", QString("Лог файл игры %1 отсутствует!").arg(currentItem->text()));
        LOG_ERR(QString("У игры %1 отсутвует лог").arg(currentItem->text()));
        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(path.value()));
}

// ============================================================================
// Перехват нажатий клавиш в окне TopWindow
// ----------------------------------------------------------------------------
// Обрабатываем Enter — переводим фокус в headerText, служебные клавиши — игнорируем.
// ============================================================================
void TopWindow::keyPressEvent(QKeyEvent *ev)
{
    // Если нажали Enter/Return — переводим фокус в headerText (так сделано в оригинале)
    if (ev->key() == Qt::Key_Return || ev->key() == Qt::Key_Enter)
    {
        ui->headerText->setFocus();
        return;
    }

    // Защита: не мешаем стандартным Ctrl+A, используем проверку сочетания.
    if(ev->modifiers() == Qt::Key_Control && ev->key() == Qt::Key_A)
    {
        return;
    }

    // Игнорируем «чисто» аппаратные клавиши (shift/ctrl/alt/капс)
    if(ev->text().isEmpty() &&
        (ev->key() == Qt::Key_Shift ||
         ev->key() == Qt::Key_Control ||
         ev->key() == Qt::Key_Alt ||
         ev->key() == Qt::Key_CapsLock))
    {
        return;
    }

    // Во всех остальных случаях переводим фокус в поле ввода (ui->enterText)
    ui->enterText->setFocus();

    // Обновляем информацию системы доступности (чтобы экранный читалка знал о новом фокусе)
    QAccessibleEvent aEvent(ui->enterText, QAccessible::Focus);
    QAccessible::updateAccessibility(&aEvent);

    // Если есть текстовый символ — вставляем его в поле ввода
    if(!ev->text().isEmpty()) ui->enterText->insert(ev->text());
}

// ============================================================================
// Фильтр событий для headerText
// ----------------------------------------------------------------------------
// Позволяет контролировать навигационные клавиши/копирование и т.д.
// ============================================================================
bool TopWindow::eventFilter(QObject *obj, QEvent *event)
{
    // Отлавливаем только keypress'ы целевого виджета ui->headerText
    if(obj == ui->headerText && event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (obj == ui->headerText)
        {
            // Если держим Ctrl — даём стандартную обработку для сочетаний (Ctrl+A/C/V и т.д.)
            if(keyEvent->modifiers() == Qt::ControlModifier)
            {
                switch(keyEvent->key())
                {
                case Qt::Key_A:
                case Qt::Key_Z:
                case Qt::Key_C:
                    QMainWindow::keyPressEvent(keyEvent);
                    return false; // позволяем дальнейшую обработку
                }
            }
            // Навигационные клавиши — не мешаем им (например, PgUp/PgDown/стрелки и т.д.)
            switch(keyEvent->key())
            {
            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_Left:
            case Qt::Key_Right:
            case Qt::CTRL:
            case Qt::Key_Home:
                return false; // стандартная обработка
                break;
            default:
                // Для всех прочих клавиш переиспользуем QMainWindow::keyPressEvent и заявляем,
                // что событие обработано (не передаём дальше).
                QMainWindow::keyPressEvent(keyEvent);
                return true;
                break;
            }
        }
    }
    // Во всех остальных случаях используем стандартный фильтр событий базового класса
    return QMainWindow::eventFilter(obj, event);
}
