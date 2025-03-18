#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>
#include <QShortcut>
#include <QShortcut>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDialog>

namespace Ui {
class Manager;
}

class Manager : public QWidget
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

public slots:
    void onPushEcho();

signals:
    void startEchoGame();

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Manager *ui;
    void keyPressEvent(QKeyEvent* event);
};

#endif // MANAGER_H
