#ifndef MANAGER_H
#define MANAGER_H

#include <QDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QShortcut>
#include <QWidget>
#include <QTest>

namespace Ui {
class Manager;
}

class TopWindow;

class Manager : public QWidget
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

    QVector<QString> namePlugin{};
    void updateLists();

signals:
    void startGame();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Manager *ui;
    TopWindow *topWindow;
    void keyPressEvent(QKeyEvent *event);
};

#endif // MANAGER_H
