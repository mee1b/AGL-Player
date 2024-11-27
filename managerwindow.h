#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H

#include <QMainWindow>
#include <QShortcut>

QT_BEGIN_NAMESPACE
namespace Ui {
class ManagerWindow;
}
QT_END_NAMESPACE

class ManagerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ManagerWindow(QWidget *parent = nullptr);
    ~ManagerWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::ManagerWindow *ui;
    std::vector<QString> tableName = {"Маленькие зомби", "Пушкин", "Доступна", "Можно играть", "1.0.0", "150 Мбит", "зомби", "27.11.2024"};
    std::vector<QString> tableName_2 = {"Картошка", "Маленький автор", "Доступна", "Можно играть", "5.0.1", "12 Мбит", "Картошка", "27.11.2024"};
};
#endif // MANAGERWINDOW_H
