#include "topwindow.h"
#include "ui_topwindow.h"
#include <QAccessible>
#include <QLabel>


TopWindow::TopWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TopWindow)
{
    ui->setupUi(this);
    ui->enterText->setAccessibleName("Поле ввода");
    reference = loadReferenceFromJson();
    mw = std::make_unique<Manager>();
    setWindowTitle("AGL-Manager");
    setMinimumSize(781, 491);
    ui->headerText->installEventFilter(this);

    createActionsName();

    if(!loadPlugin())
    {
        QMessageBox::information(this, "Ошибка!", "Не удалось загрузить плагин!");
    }
    mw->updateLists();
    managerOpen();

    QListWidget* pluginList = mw->getPlugList();
    connect(pluginList, &QListWidget::itemActivated, this, &TopWindow::onPlugSelected);

    connect(qApp, &QApplication::focusChanged, this, [this](QWidget* oldWidget, QWidget* newWidget)
            {
        if(oldWidget == ui->headerText && newWidget == ui->enterText) talkNVDA_.stopSpeak();
        if(newWidget == ui->headerText) talkNVDA_.speakTextNVDA(ui->headerText->toPlainText());
    }); //Здесь ловим смену фокуса через QApplication::focusChanged, чтобы управлять озвучкой
}

TopWindow::~TopWindow()
{
    delete ui;
}


void TopWindow::onPlugSelected(QListWidgetItem* item)
{
    if(!item) return;

    QString plugName = item->text();

    for(int i = 0; i < mw->namePlugin.size(); ++i)
    {
        if(mw->namePlugin[i] == plugName)
        {
            gameInterface = pluginsInterface[i];
            if(gameInterface)
            {
                announceSetText(ui->headerText, gameInterface->startMessage());
                disconnect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::sendEcho);
                connect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::sendEcho);
            }

            else
            {
                QMessageBox::warning(this, "Ошибка", "Плагин не реализует GameInterface");
            }
            break;
        }
    }
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
    pluginsInterface.clear();
    pluginsLoad.clear();
    pluginsDir.cd("plugins");
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString& fileName : entries)
    {
        if(!fileName.endsWith(".dll", Qt::CaseInsensitive)) {continue;}
        QString fullPath = pluginsDir.absoluteFilePath(fileName);
        QPluginLoader* pluginLoad = new QPluginLoader(fullPath, this);
        QObject* plugin = pluginLoad->instance();

        QJsonObject meta = pluginLoad->metaData().value("MetaData").toObject();
        QString displayName = meta.value("name").toString();
        if(displayName.isEmpty())
            displayName = fileName;

        if(!plugin)
        {
            delete pluginLoad;
            continue;
        }

        mw->namePlugin.push_back(displayName);

        gameInterface = qobject_cast<GameInterface*>(plugin);
        if(gameInterface)
        {
            pluginsLoad.push_back(pluginLoad);
            pluginsInterface.push_back(gameInterface);
        }
        else
        {
            delete pluginLoad;
            continue;
        }
    }

    return !pluginsLoad.isEmpty();
}

void TopWindow::announceSetText(QWidget *widget, const QString &text)
{
    if (auto* plain = qobject_cast<QPlainTextEdit*>(widget))
    {
        plain->setPlainText(text);
        plain->setFocus();
        if(talk_.currentReader() == "")
        {
            talkNVDA_.speakWithFallback(ui->headerText, text);
        }
        else
        {
            talk_.output(text, true);
            // QTimer::singleShot(1000, [widget, text]
            //                    {
            //                        QAccessibleTextUpdateEvent ev(widget, 0, "", text);
            //                        QAccessible::updateAccessibility(&ev);
            //                        QAccessibleEvent focusEvent(widget, QAccessible::Focus);
            //                        QAccessible::updateAccessibility(&focusEvent);
            //                    });
        }
    }
    // else if (auto* edit = qobject_cast<QLineEdit*>(widget))
    // {
    //     edit->setText(text);
    // }
}

QString TopWindow::loadReferenceFromJson()
{
    QFile file(":/reference.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("Ну удалось открыть reference.json");
        return {};
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if(!doc.isObject())
    {
        qWarning("reference.json не содержит JSON-Object");
        return {};
    }

    QJsonObject obj = doc.object();
    return obj.value("reference").toString();
}

void TopWindow::managerOpen()
{
    talkNVDA_.stopSpeak();
    ui->enterText->clear();
    connect(mw.get(), &Manager::closeManagerWindow, this, [this](){
            announceSetText(ui->headerText, reference);
    });
    disconnect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::sendEcho);
    mw->show();
    mw->setFocus();
    QTimer::singleShot(300, [=]()
                       {
                           mw->getPlugList()->setFocus();
                           mw->getPlugList()->setCurrentRow(0);
                       }); // асинхронно ставим фокус, для выделения первой строки списка и убираем этим двойное проговаривание менеджер игр окно
    talkNVDA_.speakTextNVDA(mw->namePlugin[0]);
}

void TopWindow::exit()
{
    QApplication::quit();
}

void TopWindow::sendEcho()
{
    if(!gameInterface) return;


    announceSetText(ui->enterText, ui->enterText->text());
    QString text = gameInterface->gameInput(ui->enterText->text());
    ui->enterText->clear();

    if(gameInterface->isOver())
    {
        QMessageBox::information(this, "End game", tr("Спасибо за игру!"));
        ui->enterText->clear();
        announceSetText(ui->headerText, reference);
        disconnect(ui->enterText, &QLineEdit::returnPressed, this, &TopWindow::sendEcho);
        return;
    }

    announceSetText(ui->headerText, text);
}

void TopWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Return || ev->key() == Qt::Key_Enter)
    {
        ui->headerText->setFocus();
        return;
    }

    if(ev->modifiers() == Qt::Key_Control && ev->key() == Qt::Key_A)
    {
        return;
    }
    if(ev->text().isEmpty() &&
        (ev->key() == Qt::Key_Shift ||
        ev->key() == Qt::Key_Control ||
        ev->key() == Qt::Key_Alt ||
        ev->key() == Qt::Key_CapsLock))
    {
        return;
    }

    ui->enterText->setFocus();
    QAccessibleEvent aEvent(ui->enterText, QAccessible::Focus);
    QAccessible::updateAccessibility(&aEvent);

    if(!ev->text().isEmpty()) ui->enterText->insert(ev->text());
}

bool TopWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->headerText && event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (obj == ui->headerText)
        {
            if(keyEvent->modifiers() == Qt::ControlModifier)
            {
                switch(keyEvent->key())
                {
                case Qt::Key_A:
                case Qt::Key_Z:
                case Qt::Key_C:
                    QMainWindow::keyPressEvent(keyEvent);
                    return false;
                }
            }
            switch(keyEvent->key())
            {
            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_Left:
            case Qt::Key_Right:
            case Qt::CTRL:
            case Qt::Key_Home:
                return false;
                break;
            default:
                QMainWindow::keyPressEvent(keyEvent);
                return true;
                break;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

