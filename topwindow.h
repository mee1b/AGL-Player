#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QLineEdit>
#include <QEvent>
#include <QString>
#include <QTest>
#include "manager.h"
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

public slots:
    void startGame();

signals:
    void start();

private slots:
    void managerOpen();
    void exit();
    void sendEcho();

private:
    Ui::TopWindow* ui;
    Manager* mw;
    EchoInterface* echoInterface;
    void keyPressEvent(QKeyEvent *ev) override;
    void createActionsName();
    bool loadPlugin();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif // TOPWINDOW_H
