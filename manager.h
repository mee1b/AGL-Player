#pragma once

#include <QDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QShortcut>
#include <QWidget>
#include <QListWidget>
#include <QEvent>

namespace Ui {
class Manager;
}

class Manager : public QWidget
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr); // Конструктор, инициализация UI и сигналов/слотов
    ~Manager(); // Деструктор, очистка ресурсов

    QVector<QString> namePlugin{}; // Список имен загруженных плагинов (игр)

    // Метод для обновления UI списка плагинов в менеджере
    // Берёт данные из namePlugin и отображает их в QListWidget
    void updateLists();

    // Возвращает указатель на QListWidget с плагинами
    // Используется для подключения сигналов, получения выбранного элемента и т.д.
    QListWidget* getPlugList() const;
    QListWidget* getPlugList2() const;

signals:
    void closeManagerWindow(); // Сигнал о закрытии окна менеджера
    void deletePlugin(QListWidgetItem* item); // Сигнал запроса на удаление выбранного плагина
    void update(); // Сигнал для обновления списка плагинов (например, после добавления новых)
    void loadGame(const QString& name);

private slots:
    void s_StartGame();       // Слот: запуск выбранной игры
    void s_Delete();          // Слот: удаление выбранного плагина
    void s_Update();          // Слот: обновление списка плагинов
    void s_LastSaveContin();  // Слот: продолжить игру с последнего сохранения

private:
    Ui::Manager *ui; // Указатель на UI, сгенерированный из .ui файла

    void keyPressEvent(QKeyEvent *event) override; // Обработка нажатий клавиш в окне менеджера
    void closeEvent(QCloseEvent* event) override;  // Обработка закрытия окна (для emit closeManagerWindow)
};
