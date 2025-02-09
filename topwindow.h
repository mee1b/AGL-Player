#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QLineEdit>
#include <QEvent>
#include "managerwindow.h"
#include "echo.h"

namespace Ui {
class TopWindow;
}

class TopWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TopWindow(QWidget *parent = nullptr);
    ~TopWindow();
    QVector<QString> namePlugin;

private slots:
    void managerOpen();
    void exit();
    void sendEcho();

private:
    Ui::TopWindow* ui;
    ManagerWindow* mw;
    EchoInterface* echoInterface;
    void keyPressEvent(QKeyEvent *ev);
    void createActionsName();
    bool loadPlugin();
};

#endif // TOPWINDOW_H
