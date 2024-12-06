#include <QtWidgets>

#include "mainwindow.h"



/*Мы начинаем с включения <QtWidgets> — заголовочного файла, содержащего определения всех классов в модулях Qt Core, Qt GUI и Qt Widgets.
 Это избавляет нас от необходимости включать каждый класс по отдельности.
 Мы также включаем mainwindow.h.

Вы можете задаться вопросом, почему мы не включаем <QtWidgets> в mainwindow.h и не заканчиваем на этом.
Причина в том, что включение такого большого заголовка из другого заголовочного файла может значительно снизить производительность.
В данном случае это не причинит вреда, но в целом лучше включать только те заголовочные файлы, которые строго необходимы из другого заголовочного файла.*/

MainWindow::MainWindow()
    : textEdit(new QPlainTextEdit)
{
    setWindowTitle("AGL-Manager");
    setCentralWidget(textEdit);
    setMinimumSize(850, 550);
    textEdit->appendPlainText("Краткая справка:\n"
                              "Для начала новой игры из библиотеки нажмите CTRL+M и выберите игру и нажмите ENTER.\n"
                              "Если вы скачали игру, которой нет в библиотеке, то нажмите CTRL+N и выберите архив с игрой.\n"
                              "Для того чтобы играть выберите пункт в одном из списков - объекты, инвентарь, пути и нажмите клавишу ENTER.\n"
                              "Когда вы находитесь в инвентаре, вам необходимо сначала выбрать пункт, нажать ENTER, а затем выбрать второй пункт и нажать ENTER.\n"
                              "Поле ввода служит для ввода текста в метапарсерных играх (только instead 3),или для ввода команд,если в этом есть необходимость.\n"
                              "В метапарсерных играх,скорее всего,взаимодействие с инвентарём не будет работать.\n"
                              "Если при взаимодействием с объектами или при вводе текста ничего не изменилось,значит,скорее всего,текст остался тем же,какой был перед взаимодействием с объектами или ввода команды.\n"
                              "Сохранение и загрузка игр происходит обязательно в каталоге с текущей игрой (иногда в подкаталоге autosaves,если игра сама сохраняет своё состояние), менять на другой нельзя.\n"
                              "При сохранении, указывайте пожалуйста имя файла латинскими буквами или цифрами.\n"
                              "Внимание! Большие архивы могут не распаковываться программой через менеджер или установку в библиотеку. Попробуйте самостоятельно распаковать их в папку с играми.\n");
    textEdit->setReadOnly(true);
    textEdit->zoomIn(3);

    QLineEdit *enterText = new QLineEdit;
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(enterText);
    textEdit->setLayout(layout);
    layout->setAlignment(enterText, Qt::AlignBottom);
    connect(enterText, SIGNAL(returnPressed()), SLOT(tapText()));




    createActions();
    createStatusBar();

    readSettings();

    connect(textEdit->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);

#ifndef QT_NO_SESSIONMANAGER
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}
/*
 В конструкторе мы начинаем с создания виджета QPlainTextEdit в качестве дочернего элемента главного окна (объекта this).
 Затем мы вызываем QMainWindow::setCentralWidget(), чтобы сообщить, что это будет виджет,
 занимающий центральную область главного окна между панелями инструментов и строкой состояния.

Затем мы вызываем createActions() и createStatusBar(), две частные функции, которые настраивают пользовательский интерфейс.
После этого мы вызываем readSettings() для восстановления настроек пользователя.

Мы устанавливаем сигнально-слотовую связь между объектом документа QPlainTextEdit и нашим слотом documentWasModified().
Всякий раз, когда пользователь изменяет текст в QPlainTextEdit, мы хотим обновить строку заголовка, чтобы показать, что файл был изменён.

В конце мы задаём заголовок окна с помощью приватной функции setCurrentFile(). Мы вернёмся к этому позже.
*/
void MainWindow::tapText()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
/*
 Когда пользователь пытается закрыть окно, мы вызываем закрытую функцию maybeSave(), чтобы дать пользователю возможность сохранить ожидающие обработки изменения.
Функция возвращает true, если пользователь хочет закрыть приложение; в противном случае она возвращает false.
В первом случае мы сохраняем настройки пользователя на диске и принимаем событие закрытия;
во втором случае мы игнорируем событие закрытия, то есть приложение продолжит работать, как будто ничего не произошло.
*/
void MainWindow::newFile()
{
    ManagerWindow* mw = new ManagerWindow;
    mw->show();
}
/*
Слот newFile() вызывается, когда пользователь выбирает File|New в меню.
Мы вызываем maybeSave() для сохранения ожидающих обработки изменений, и если пользователь соглашается продолжить,
мы очищаем QPlainTextEdit и вызываем приватную функцию setCurrentFile() для обновления заголовка окна и очистки флага windowModified.
*/

void MainWindow::open()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty()) { loadFile(fileName); }
    }
}
/*Слот open() вызывается, когда пользователь нажимает File|Open. Мы выводим QFileDialog с запросом к пользователю выбрать файл.
Если пользователь выбирает файл (т. е. fileName не является пустой строкой), мы вызываем закрытую функцию loadFile() для загрузки файла.*/

bool MainWindow::save()
{
    if (curFile.isEmpty()) { return saveAs(); }
    else { return saveFile(curFile); }
}
/*Слот save() вызывается, когда пользователь нажимает File|Save.
 Если пользователь ещё не ввёл имя файла, мы вызываем saveAs(); в противном случае мы вызываем приватную функцию saveFile() для сохранения файла.*/

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if(dialog.exec() != QDialog::Accepted) return false;
    return saveFile(dialog.selectedFiles().first());
}
/*
 В saveAs() мы начинаем с того, что запрашиваем у пользователя QFileDialog имя.
Если пользователь нажимает Cancel, возвращаемое имя файла будет пустым, и мы ничего не будем делать.
*/

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The <b>Aplication<b> example demonstrates how to "
                          "write modern GUI applications using Qt, with a menu bar, "
                          "Toolbars, and a status bar."));
}
/*Окно «О программе» в приложении создаётся с помощью одного оператора, статической функции QMessageBox::about() и её поддержки подмножества HTML.

Вызов tr() вокруг строковой константы помечает строку для перевода. Рекомендуется вызывать tr() для всех видимых пользователю строк на случай,
если вы решите перевести приложение на другие языки. Обзор Internationalization with Qt более подробно описывает tr()*/

void MainWindow::documentWasModified()
{
    setWindowModified(textEdit->document()->isModified());
}
/*
Слот documentWasModified() вызывается каждый раз, когда текст в QPlainTextEdit изменяется из-за правок пользователя.
Мы вызываем QWidget::setWindowModified(), чтобы в строке заголовка отображалось, что файл был изменён.
Способ, которым это делается, зависит от платформы.
*/

void MainWindow::exit()
{
    QWidget::close();
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&Файл"));
    QAction *gameManager = new QAction(tr("Менеджер игр"), this);
    gameManager->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    gameManager->setStatusTip(tr("Открыть менеджер игр"));
    connect(gameManager, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(gameManager);
    QAction *addGame = new QAction(tr("Добавить игру в библиотеку"), this);
    addGame->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    addGame->setStatusTip(tr("Добавить игру в библиотеку"));
    fileMenu->addAction(addGame);
    QAction *openSave = new QAction(tr("Открыть сохранение"), this);
    openSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    openSave->setStatusTip(tr("Открыть сохранение"));
    fileMenu->addAction(openSave);
    QAction *openSaveDontReload = new QAction(tr("Открыть сохранение без перезагрузки библиотеки"), this);
    openSaveDontReload->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_O));
    openSaveDontReload->setStatusTip(tr("Открыть сохранение без перезагрузки библиотеки"));
    fileMenu->addAction(openSaveDontReload);
    QAction *saveGame = new QAction(tr("Сохранить игру"), this);
    saveGame->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    saveGame->setStatusTip(tr("Сохранить игру"));
    fileMenu->addAction(saveGame);
    QAction *startGameAgain = new QAction(tr("Начать игру заново"), this);
    startGameAgain->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    startGameAgain->setStatusTip(tr("Начать игру заново"));
    fileMenu->addAction(startGameAgain);
    fileMenu->addSeparator();
    QAction *resetAllSettings = new QAction(tr("Сбросить все настройки"), this);
    resetAllSettings->setStatusTip(tr("Сбросить все настройки"));
    resetAllSettings->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Z);
    fileMenu->addAction(resetAllSettings);
    QAction *exit = new QAction(tr("Выход"), this);
    exit->setStatusTip(tr("Выход"));
    exit->setShortcut(QKeySequence::Close);
    connect(exit, &QAction::triggered, this, &MainWindow::exit);
    fileMenu->addAction(exit);


    QMenu *showMenu = menuBar()->addMenu(tr("&Просмотр"));
    QAction *setting = addAction(tr("Настройка"));
    setting->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_T));
    setting->setStatusTip("Настройка");
    showMenu->addAction(setting);
    QAction *search = addAction(tr("Поиск"));
    search->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
    search->setStatusTip(tr("Поиск"));
    showMenu->addAction(search);
    QAction *searchFurther = addAction(tr("Найти далее"));
    searchFurther->setShortcut(QKeySequence(Qt::Key_F1));
    searchFurther->setStatusTip(tr("Найти далее"));
    showMenu->addAction(searchFurther);
    QAction *sceneText = addAction(tr("Текст сцены"));
    sceneText->setShortcut(QKeySequence(Qt::Key_F2));
    sceneText->setStatusTip(tr("Текст сцены"));
    showMenu->addAction(sceneText);
    QAction *objectList = addAction(tr("Перейти к списку объектов"));
    objectList->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_1));
    objectList->setStatusTip(tr("Перейти к списку объектов"));
    showMenu->addAction(objectList);
    QAction *inventList = addAction(tr("Перейти к списку инвентаря"));
    inventList->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_2));
    inventList->setStatusTip(tr("Перейти к списку инвентаря"));
    showMenu->addAction(inventList);
    QAction *wayList = addAction(tr("Перейти к списку путей"));
    wayList->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_3));
    wayList->setStatusTip(tr("Перейти к списку путей"));
    showMenu->addAction(wayList);
    QAction *debugMode = addAction(tr("Режим отладки"));
    debugMode->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    debugMode->setStatusTip(tr("Режим отладки"));
    showMenu->addAction(debugMode);
    QAction *logOn = addAction(tr("Включить логирование"));
    logOn->setShortcut(QKeySequence::Close);
    logOn->setStatusTip(tr("Включить логирование"));
    showMenu->addAction(logOn);

    QMenu *soundMenu = menuBar()->addMenu(tr("&Звук"));
    QAction *musicUp = addAction(tr("Музыка громче"));
    musicUp->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F1));
    musicUp->setStatusTip(tr("Музыка громче"));
    soundMenu->addAction(musicUp);
    QAction *musicLow = addAction(tr("Музыка тише"));
    musicLow->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F2));
    musicLow->setStatusTip(tr("Музыка тише"));
    soundMenu->addAction(musicLow);
    QAction *switchMusic = addAction(tr("Музыка включена"));
    switchMusic->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F3));
    switchMusic->setStatusTip(tr("Музыка включена"));
    switchMusic->setCheckable(true);
    soundMenu->addAction(switchMusic);
    QAction *soundUp = addAction(tr("Звуки громче"));
    soundUp->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F1));
    soundUp->setStatusTip(tr("Звуки громче"));
    soundMenu->addAction(soundUp);
    QAction *soundLow = addAction(tr("Звуки Тише"));
    soundLow->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F2));
    soundLow->setStatusTip(tr("Звуки Тише"));
    soundMenu->addAction(soundLow);
    QAction *switchSound = addAction(tr("Звуки включены"));
    switchSound->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F3));
    switchSound->setStatusTip(tr("Звуки включены"));
    switchSound->setCheckable(true);
    soundMenu->addAction(switchSound);
    QAction *alertUp = addAction(tr("Уведомления громче"));
    alertUp->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_5));
    alertUp->setStatusTip(tr("Уведомления громче"));
    soundMenu->addAction(alertUp);
    QAction *alertLow = addAction(tr("Уведомления тише"));
    alertLow->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_6));
    alertLow->setStatusTip(tr("Уведомления тише"));
    soundMenu->addAction(alertLow);
    QAction *switchAlert = addAction(tr("Уведомления включены"));
    switchAlert->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_7));
    switchAlert->setStatusTip(tr("Уведомления включены"));
    switchAlert->setCheckable(true);
    soundMenu->addAction(switchAlert);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Справка"));
    QAction *findOutput = addAction(tr("Текущий способ вывода"));
    findOutput->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_M));
    findOutput->setStatusTip(tr("Текущий способ вывода"));
    helpMenu->addAction(findOutput);

    /*#ifndef QT_NO_CLIPBOARD
    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
    connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
}
Действия Edit|Cut и Edit|Copy должны быть доступны только в том случае, если QPlainTextEdit содержит выделенный текст.
 * Мы отключаем их по умолчанию и подключаем сигнал QPlainTextEdit::copyAvailable() к слоту QAction::setEnabled(),
 * чтобы действия отключались, когда в текстовом редакторе нет выделенного текста.

Непосредственно перед созданием меню Help мы вызываем QMenuBar::addSeparator().
Это не влияет на большинство стилей виджетов (например, стили Windows и macOS),
но для некоторых стилей это гарантирует, что Help будет перемещён в правую часть строки меню.*/
} // Для примера только 1 менюшка и один action, доделать!!!
/*
createActions() Закрытая функция, которая вызывается из MainWindow конструктора,
создает QActionы и заполняет меню и две панели инструментов.
Код очень повторяющийся, поэтому мы показываем только действия, соответствующие File|New, File|Open и Help|About Qt.

QAction — это объект, представляющий одно действие пользователя, например сохранение файла или вызов диалогового окна.
Действие может быть помещено в QMenu или в QToolBar, или в оба, или в любой другой виджет, реализующий QWidget::actionEvent().

Действие содержит текст, который отображается в меню, значок, сочетание клавиш, всплывающую подсказку, подсказку о состоянии (отображается в строке состояния),
текст «Что это такое?» и многое другое. Оно генерирует triggered() сигнал всякий раз, когда пользователь запускает действие
(например, нажав на соответствующий пункт меню или кнопку на панели инструментов).

Экземпляры QAction можно создать, передав родительский QObject или используя одну из удобных функций QMenu, QMenuBar или QToolBar.
Мы создаём действия, которые находятся в меню, а также в панели инструментов, привязанной к окну, чтобы избежать проблем с владением.
Для действий, которые находятся только в меню, мы используем удобную функцию QMenu::addAction(),
которая позволяет передавать текст, значок и целевой объект, а также функцию-член слота.

Создание панелей инструментов очень похоже на создание меню.
Те же действия, которые мы помещаем в меню, можно использовать повторно на панелях инструментов.
После создания действия мы добавляем его на панель инструментов с помощью QToolBar::addAction().

Приведённый выше код содержит ещё одну идиому, которую необходимо объяснить.
Для некоторых действий мы указываем значок в качестве QIcon в конструкторе QAction.
Мы используем QIcon::fromTheme() для получения правильного стандартного значка из базовой оконной системы.
Если это не удаётся из-за того, что платформа не поддерживает его, мы передаём имя файла в качестве запасного варианта.
Здесь имя файла начинается с :. Такие имена файлов — это не обычные имена файлов, а пути к ресурсам, хранящимся в исполняемом файле.
Мы вернёмся к этому, когда рассмотрим файл application.qrc, который является частью проекта.
*/

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Добро пожаловать в AGL-Manager!"));
}
/*
 * QMainWindow::statusBar() возвращает указатель на виджет QStatusBar главного окна.
 * Как и в случае с QMainWindow::menuBar(), виджет автоматически создаётся при первом вызове функции.
*/

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
    }
}
/*Функция readSettings() вызывается из конструктора для загрузки пользовательских настроек и других параметров приложения.
 * Класс QSettings предоставляет высокоуровневый интерфейс для постоянного хранения настроек на диске.
 * В Windows он использует (не)известный реестр Windows; в macOS — собственный API CFPreferences на основе XML; в Unix/X11 — текстовые файлы.

Конструктор QSettings принимает аргументы, которые идентифицируют вашу компанию и название продукта.
Это позволяет хранить настройки для разных приложений отдельно.

Мы используем QSettings::value() для извлечения значения параметра геометрии.
Второй аргумент QSettings::value() является необязательным и указывает значение параметра по умолчанию, если оно отсутствует.
Это значение используется при первом запуске приложения.

Мы используем QWidget::saveGeometry() и Widget::restoreGeometry() для сохранения положения.
Они используют непрозрачный QByteArray для хранения номера экрана, геометрии и состояния окна.*/

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}
/*Функция writeSettings() вызывается из closeEvent().
 * Запись настроек похожа на их чтение, только проще.
 * Аргументы конструктора QSettings должны быть такими же, как в readSettings().*/

bool MainWindow::maybeSave()
{
    if (!textEdit->document()->isModified()) { return true; }
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Appliacation"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}
/*Функция maybeSave() вызывается для сохранения ожидающих изменений.
 * Если ожидающие изменения есть, появляется сообщениеQMessageBox, позволяющее пользователю сохранить документ.
 * Возможны следующие опции: QMessageBox::Yes, QMessageBox::No и QMessageBox::Cancel.
 * Yes Кнопка становится кнопкой по умолчанию (кнопка, которая вызывается при нажатии пользователем Return) с использованием QMessageBox::Default флага;
 * Cancel кнопка становится кнопкой выхода (кнопка, которая вызывается при нажатии пользователем Esc) с использованием QMessageBox::Escape флага.

Функция maybeSave() возвращает true во всех случаях, кроме тех, когда пользователь нажимает Cancel или сохранение файла не удаётся.
Вызывающая сторона должна проверить возвращаемое значение и прекратить выполнение, если возвращаемое значение равно false.*/

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}
/*В loadFile() мы используем QFile и QTextStream для считывания данных.
 *  Объект QFile обеспечивает доступ к байтам, хранящимся в файле.

Начнём с открытия файла в режиме только для чтения. Флаг QFile::Text указывает на то, что файл является текстовым, а не двоичным.
В Unix и macOS это не имеет значения, но в Windows это гарантирует, что последовательность символов конца строки "\r\n"
при чтении преобразуется в "\n".

Если мы успешно открыли файл, мы используем объект QTextStream для считывания данных.
QTextStream автоматически преобразует 8-битные данные в Unicode QString и поддерживает различные кодировки.
Если кодировка не указана, QTextStream предполагает, что файл записан с использованием 8-битной кодировки системы по умолчанию
(например, Latin-1; подробности см. в QTextCodec::codecForLocale()).

Поскольку вызов QTextStream::readAll() может занять некоторое время,
мы устанавливаем курсор в положение Qt::WaitCursor для всего приложения, пока оно работает.

В конце мы вызываем приватную функцию setCurrentFile(), о которой поговорим чуть позже,
и выводим в строке состояния строку «Файл загружен» на 2 секунды (2000 миллисекунд).
*/

bool MainWindow::saveFile(const QString &fileName)
{
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        if (!file.commit())
        {
            errorMessage = tr("Cannot write file %1:\n%2.")
                               .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    }
    else
    {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                        .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("Application"), errorMessage);
        return false;
    }

        setCurrentFile(fileName);
        statusBar()->showMessage(tr("File saved"), 2000);
        return true;
}
/*Сохранение файла похоже на его загрузку.
 * Мы используем QSaveFile для того, чтобы все данные были записаны безопасно, а существующие файлы не были повреждены в случае сбоя при записи.
 * Мы используем флаг QFile::Text для того, чтобы в Windows символ "\n" преобразовывался в "\r\n" в соответствии с соглашением Windows.
*/

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString showName = curFile;
    if (curFile.isEmpty()){ showName = "name.txt"; }
    setWindowFilePath(showName);
}
/*Функция setCurrentFile() вызывается для сброса состояния нескольких переменных при загрузке или сохранении файла,
 * а также при начале редактирования пользователем нового файла (в этом случае fileName пусто).
 * Мы обновляем переменную curFile, очищаем флаг QTextDocument::modified и связанный с ним флаг QWidget:windowModified и обновляем заголовок окна,
 * чтобы он содержал новое имя файла (или untitled.txt).
*/

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
/*Вызов функции strippedName() внутри curFile в вызове QWidget::setWindowTitle()
 * сокращает имя файла, исключая путь. Вот эта функция*/

void MainWindow::commitData(QSessionManager &manager)
{
    if (manager.allowsInteraction())
    {
        int ret = QMessageBox::warning(
            this,
            tr("My Application"),
            tr("Save changes to document?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        switch (ret)
        {
        case QMessageBox::Save:
            manager.release();
            if (!maybeSave())
                manager.cancel();
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
        default:
            manager.cancel();
        }
    }

}
