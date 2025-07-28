#pragma once

#include <QDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QShortcut>
#include <QWidget>
#include <QListWidget>
#include <QEvent>
#include "NVDAController.h"
#include "talkwrap.h"

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
    //метод для загрузки имен плагинов в менеджер
    void updateLists();
    QListWidget* getPlugList() const;

signals:
    void closeManagerWindow();

private slots:
    void s_StartGame();
    void s_Delete();
    void s_Update();
    void s_LastSaveContin();

private:
    Ui::Manager *ui;
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent* event) override;
};
