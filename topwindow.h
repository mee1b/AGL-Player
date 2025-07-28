#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QLineEdit>
#include <QEvent>
#include <QString>
#include <QTimer>
#include <memory>
#include <QAccessible>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "manager.h"
#include "echo.h"
#include "basketball.h"


class TalkWrap;

namespace Ui {
class TopWindow;
}

class TopWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TopWindow(QWidget *parent = nullptr);
    ~TopWindow();

private:
    void keyPressEvent(QKeyEvent *ev) override;
    void createActionsName();
    //метод загрузки плагинов
    bool loadPlugin();
    //Главный метод озвучки приложения
    void announceSetText(QWidget *widget, const QString &text);
protected:
    QString loadReferenceFromJson();
    bool eventFilter(QObject* obj, QEvent* event) override;

public slots:
    //слот для выбора имя плагина = игра плагин
    void onPlugSelected(QListWidgetItem* item);

private slots:
    void managerOpen();
    void exit();
    void sendEcho();

private:
    Ui::TopWindow* ui;
    std::unique_ptr<Manager> mw;
    GameInterface* gameInterface = nullptr;
    //вектор плагинов
    QVector<QPluginLoader*> pluginsLoad;
    //вектор плагинов реализующих GameInterface
    QVector<GameInterface*> pluginsInterface;
    QString reference;
    TalkWrap talk_;
    Speaker talkNVDA_;
    bool speakFlag = false;
};

