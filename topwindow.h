#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QLineEdit>
#include <QEvent>
#include <QString>
#include <QTest>
#include <QTimer>
#include <QAccessible>
#include "manager.h"
#include "echo.h"
#include "basketball.h"

namespace Ui {
class TopWindow;
}

class TopWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TopWindow(QWidget *parent = nullptr);
    ~TopWindow();

public slots:
    void startGameEcho();
    void onPlugSelected(QListWidgetItem* item);

private slots:
    void managerOpen();
    void exit();
    void sendEcho();

private:
    Ui::TopWindow* ui;
    Manager* mw;
    GameInterface* gameInterface = nullptr;
    QVector<QPluginLoader*> pluginsLoad;
    QVector<GameInterface*> pluginsInterface;

    void keyPressEvent(QKeyEvent *ev) override;
    void createActionsName();
    bool loadPlugin();
    QString reference = ">Краткая справка:\n"
        "Для начала новой игры из библиотеки нажмите CTRL+M и выберите игру и нажмите ENTER.\n"
        "Если вы скачали игру, которой нет в библиотеке, то нажмите CTRL+N и выберите архив с игрой.\n"
        "Для того чтобы играть выберите пункт в одном из списков - объекты, инвентарь, пути и нажмите клавишу ENTER.\n"
        "Когда вы находитесь в инвентаре, вам необходимо сначала выбрать пункт, нажать ENTER, а затем выбрать второй пункт и нажать ENTER.\n"
        "Поле ввода служит для ввода текста в метапарсерных играх (только instead 3),или для ввода команд,если в этом есть необходимость.\nВ метапарсерных играх,скорее всего,взаимодействие с инвентарём не будет работать.\n"
        "Если при взаимодействием с объектами или при вводе текста ничего не изменилось,значит,скорее всего,текст остался тем же,какой был перед взаимодействием с объектами или ввода команды.\n"
        "Сохранение и загрузка игр происходит обязательно в каталоге с текущей игрой (иногда в подкаталоге autosaves,если игра сама сохраняет своё состояние), менять на другой нельзя.\n"
        "При сохранении, указывайте пожалуйста имя файла латинскими буквами или цифрами.\n"
        "Внимание! Большие архивы могут не распаковываться программой через менеджер или установку в библиотеку.\nПопробуйте самостоятельно распаковать их в папку с играми.";
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

