#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "managerwindow.h"

namespace Ui {
class TopWindow;
}

class TopWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TopWindow(QWidget *parent = nullptr);
    ~TopWindow();

private slots:
    void managerOpen();
    void exit();
    void tapText();

private:
    Ui::TopWindow* ui;
    ManagerWindow* mw;
    void createActionsName();
};

#endif // TOPWINDOW_H
