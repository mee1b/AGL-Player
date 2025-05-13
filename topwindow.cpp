#include "topwindow.h"
#include "ui_topwindow.h"


TopWindow::TopWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TopWindow)
{
    ui->setupUi(this);
    mw = new Manager;
    // bsks = new Basketball;
    setWindowTitle("AGL-Manager v6");
    setMinimumSize(781, 491);
    ui->headerText->installEventFilter(this);
    ui->headerText->isReadOnly();

    createActionsName();

    if(!loadPlugin())
    {
        QMessageBox::information(this, "Ошибка!", "Не удалось загрузить плагин!");
    }
    mw->updateLists();
    managerOpen();
    // connect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::sendEcho);
    connect(mw, &Manager::startGame, this, &TopWindow::startGameEcho);

}

TopWindow::~TopWindow()
{
    delete ui;
}

void TopWindow::startGameEcho()
{
    ui->headerText->setPlainText(echoInterface->startMessage());
    ui->enterText->setFocus();
    connect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::sendEcho);
}

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


    ui->startGameAgain->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    ui->startGameAgain->setStatusTip(tr("Начать игру заново"));

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

bool TopWindow::loadPlugin()
{
    QDir pluginsDir(QCoreApplication::applicationDirPath());

    pluginsDir.cd("plugins");
    pluginsDir.cd("echo");
    pluginsDir.setNameFilters(QStringList() << "*.dll");
    const QStringList entries = pluginsDir.entryList();
    for (const QString& fileName : entries)
    {
        mw->namePlugin.push_back(fileName);
        QPluginLoader pluginLoad(pluginsDir.absoluteFilePath(fileName));
        QObject* plugin = pluginLoad.instance();
        if(plugin)
        {
            echoInterface = qobject_cast<EchoInterface*>(plugin);
            if(echoInterface) return true;
            pluginLoad.unload();
        }
    }

    return false;
}

void TopWindow::managerOpen()
{
    mw->show();
}

void TopWindow::exit()
{
    QWidget::close();
}

void TopWindow::sendEcho()
{
    // ui->headerText->setPlainText(bsks->printGame());
    QString text = echoInterface->gameInput(ui->enterText->text());
    ui->enterText->clear();

    if(echoInterface->isOver())
    {
        QMessageBox::information(this, "End game", tr("Спасибо за игру!"));
        ui->headerText->setPlainText(reference);
        ui->enterText->clear();
        disconnect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::sendEcho);
        return;
    }


    ui->headerText->clear();
    ui->headerText->setFocus();
    ui->headerText->setPlainText(text);
}

void TopWindow::keyPressEvent(QKeyEvent *ev)
{
    if((ev->key() == Qt::Key_Return) || (ev->key() == Qt::Key_Enter))
    {
        ui->headerText->setFocus();
    }
    else if(ev->key() == Qt::Key_Tab || ev->key() == Qt::Key_Backspace || ev->key() == Qt::Key_Insert || ev->key() == Qt::Key_Delete)
    {
        ui->enterText->setFocus();
    }
    else
    {
        QTest::keyClick(ui->enterText, ev->key());
        ui->enterText->setFocus();
    }
}

bool TopWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->headerText)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            switch(keyEvent->key())
            {
            case Qt::Key_Up:
                return false;
                break;
            case Qt::Key_Down:
                return false;
                break;
            case Qt::Key_Left:
                return false;
                break;
            case Qt::Key_Right:
                return false;
                break;
            case Qt::CTRL:
                return false;
                break;
            case Qt::Key_Home:
                return false;
                break;
            default:
                TopWindow::keyPressEvent(keyEvent);
                return true;
                break;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

