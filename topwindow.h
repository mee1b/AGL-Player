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
#include "GameInterface.h"
#include "NVDAController.h"
#include "talkwrap.h"


class TalkWrap;

namespace Ui {
class TopWindow;
}

class TopWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TopWindow(QWidget *parent = nullptr); // Конструктор окна приложения
    ~TopWindow(); // Деструктор, очищает UI и освобождает ресурсы

private:
    void keyPressEvent(QKeyEvent *ev) override; // Обработка нажатий клавиш в окне
    void createActionsName(); // Создание действий (QAction), горячих клавиш и подсказок
    bool loadPlugin(); // Загрузка всех плагинов из папки plugins
    void announceSetText(QWidget *widget, const QString &text); // Вывод текста в виджет и установка фокуса

protected:
    QString loadReferenceFromJson(); // Загрузка справочного текста из JSON
    bool eventFilter(QObject* obj, QEvent* event) override; // Фильтр событий (например, для headerText)

public slots:
    void onPlugSelected(QListWidgetItem* item); // Выбор плагина/игры из списка
    void deletePlug(QListWidgetItem* item); // Удаление выбранного плагина
    void updatePlug(); // Обновление списка плагинов
    void againGame(); // Перезапуск текущей игры/плагина

private slots:
    void managerOpen(); // Открытие окна менеджера плагинов
    void exit(); // Выход из приложения
    void runGame(); // Запуск игры (слот для запуска плагина)

private:
    Ui::TopWindow* ui; // Указатель на UI, созданный через Qt Designer
    std::unique_ptr<Manager> mw; // Окно-менеджер плагинов, автоматическое управление памятью
    GameInterface* gameInterface = nullptr; // Интерфейс текущей игры/плагина
    QVector<QPluginLoader*> pluginsLoad; // Вектор загрузчиков плагинов (QPluginLoader)
    QVector<GameInterface*> pluginsInterface; // Вектор интерфейсов загруженных плагинов
    QVector<QString> filePaths; // Пути к файлам плагинов
    QString reference; // Справочный текст приложения
    TalkWrap talk_; // Обёртка для системы озвучки
    Speaker talkNVDA_; // Объект для работы с NVDA (озвучка)
    QListWidgetItem* currentItem = nullptr; // Текущий выбранный элемент в списке плагинов
};

