#include "manager.h"
#include "ui_manager.h"

// -------------------------
// Файл: manager.cpp
// Назначение: реализация окна-менеджера плагинов/игр.
// Комментарии в коде подробно объясняют что и почему.
// -------------------------

Manager::Manager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Manager) // ui создаётся динамически и освобождается в деструкторе
{
    // Привязываем сгенерированный класс UI к этому виджету.
    ui->setupUi(this);

    // Делаем диалог модальным по приложению — пока открыт менеджер, пользователь
    // не сможет взаимодействовать с остальными окнами приложения.
    setWindowModality(Qt::ApplicationModal);

    // Устанавливаем флаги окна: обычное окно с заголовком и кнопкой закрытия.
    // Используем сочетание флагов вместо одного setWindowFlags() -- это даёт полный контроль.
    setWindowFlags(Qt::Window | Qt::WindowTitleHint);
    setWindowFlag(Qt::WindowCloseButtonHint, true);

    // Заголовок окна
    setWindowTitle("Менеджер игр");

    // --- Настройка UI (вкладки, хоткеи, сигналы кнопок) ---

    // Переименовываем вкладки для удобства (UI создан в Designer)
    ui->switchGame->setTabText(0, "Установленные игры");
    ui->switchGame->setTabText(1, "Репозиторий игр");

    // Назначаем горячие клавиши для кнопок (доступность с клавиатуры)
    ui->b_StartGame->setShortcut(QKeySequence(Qt::Key_Return));
    ui->b_LastSaveContin->setShortcut(QKeySequence(Qt::Key_F3));
    ui->b_Delete->setShortcut(QKeySequence(Qt::Key_Delete));
    ui->b_Updata->setShortcut(QKeySequence(Qt::Key_F5));

    // Подключаем слоты к сигналам кнопок — клики вызывают локальные обработчики
    connect(ui->b_StartGame, &QPushButton::clicked, this, &Manager::s_StartGame);
    connect(ui->b_LastSaveContin, &QPushButton::clicked, this, &Manager::s_LastSaveContin);
    connect(ui->b_Delete, &QPushButton::clicked, this, &Manager::s_Delete);
    connect(ui->b_Updata, &QPushButton::clicked, this, &Manager::s_Update);

    // Примечание:
    // - Здесь мы не подписываемся на сигналы списков (itemActivated и т.д.), потому что
    //   основная логика (что делать при выборе/удалении) обрабатывается в TopWindow.
    // - Manager служит "чистым" представлением и эмиттером сигналов (например deletePlugin).
}

// Деструктор: освобождаем структуру UI (все дочерние объекты UI удаляются через ui)
Manager::~Manager()
{
    delete ui;
}

// updateLists — заполняет списки именами плагинов из namePlugin.
// ВАЖНО:
//  - Метод не очищает предыдущие элементы, поэтому при повторных вызовах
//    будут дубли. Можно добавить ui->plugList->clear() при необходимости.
//  - namePlugin должен быть согласован с теми коллекциями, которыми управляет TopWindow.
void Manager::updateLists()
{
    if(namePlugin.empty()) return;

    // Для каждого имени создаём элемент в обоих списках интерфейса.
    for(const auto& plugin : namePlugin)
    {
        ui->plugList->addItem(plugin);
        ui->plugList_2->addItem(plugin);
    }
}

// Возвращаем указатель на QListWidget, чтобы внешний код (TopWindow) мог с ним работать.
// Владелец списка — UI (ui), поэтому внешний код не должен delete этот указатель.
QListWidget *Manager::getPlugList() const
{
    return ui->plugList;
}

QListWidget *Manager::getPlugList2() const
{
    return ui->plugList_2;
}

// Переопределение keyPressEvent: если нажали Escape — закрываем окно.
// Это даёт привычное поведение для модальных диалогов.
void Manager::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape)
    {
        close();
    }
}

// s_StartGame — слот, вызываемый при нажатии кнопки "Start".
// Логика:
//  - Проверяем, есть ли выбранный элемент. Если нет — показываем сообщение.
//  - Берём текущий элемент и эмитим сигнал itemActivated этого списка.
//    в TopWindow  подключается к сигналу itemActivated.
//  - Скрываем менеджер (он модален, поэтому можно прятать его, чтобы вернуться в приложение).
void Manager::s_StartGame()
{
    if(ui->plugList->currentItem() == nullptr) {
        QMessageBox::information(this, tr("Info"), tr("Игра не установлена!"));
        return;
    }
    QListWidgetItem* item = ui->plugList->currentItem();

    emit ui->plugList->itemActivated(item);

    // Скрываем менеджер — управление продолжится в внешнем коде (TopWindow)
    this->hide();
}

// s_Delete — слот удаления плагина.
// Логика:
//  - Проверяем, что что-то выбрано.
//  - Берём текущий элемент и эмитим наш собственный сигнал deletePlugin(item).
//    В TopWindow этот сигнал перехватывается и выполняется фактическое удаление.
void Manager::s_Delete()
{
    if(ui->plugList->currentItem() == nullptr) {
        QMessageBox::information(this, tr("Info"), tr("Игра не установлена!!"));
        return;
    }
    //Сделать модалку с вопросом "Удалить игру?"
    //if(accept)
    QListWidgetItem* item = ui->plugList->currentItem();

    // Мы не удаляем item напрямую в Manager — Manager просто сообщает внешнему
    // коду, какой элемент нужно удалить, а TopWindow делает безопасность удаления.
    emit deletePlugin(item);
}

// s_Update — слот "Обновить".
// Логика:
//  - Выводим модальное окно, с целью подтвердить обновление
//  - Если "Да" - эмитим сигнал в главное окно
//    В TopWindow этот сигнал перехватывается и выполняется фактическое обновление.
void Manager::s_Update()
{
    //Сделать модалку с вопросом "Обновить список игр?"
    //if(accept)
    emit update();
    //else
    //return
}

// s_LastSaveContin — заглушка для кнопки "Продолжить с последнего сохранения".
void Manager::s_LastSaveContin()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}

// closeEvent — переопределён, чтобы прокинуть сигнал closeManagerWindow при закрытии.
// TopWindow подписывается на этот сигнал, чтобы восстанавливать состояние и озвучивание.
void Manager::closeEvent(QCloseEvent *event)
{
    emit closeManagerWindow();
    QWidget::closeEvent(event); // вызываем базовую логику закрытия
}
