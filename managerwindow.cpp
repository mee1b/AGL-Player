#include "managerwindow.h"
#include "./ui_managerwindow.h"


ManagerWindow::ManagerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ManagerWindow)
{
    ui->setupUi(this);
    setWindowTitle("Менеджер игр");
    setWindowModality(Qt::ApplicationModal);
    //таблица 1
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setRowCount(150);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()    << "Название"
                                                                << "Автор"
                                                                << "Доступна"
                                                                << "Комментрай о доступности"
                                                                << "Версия"
                                                                << "Размер"
                                                                << "Кратко"
                                                                << "Дата"); //Установка имени столбцов
    ui->tableWidget->horizontalHeader()->setStyleSheet(
        "  QHeaderView::section {"
        "  border-width: 1px;"
        "  border-style: solid;"
        "  border-color: black;"
        "}");
    ui->tableWidget->horizontalHeader()->resizeSection(3, 180);
    ui->tableWidget->verticalHeader()->setVisible(false);
     //таблица 2
    ui->tableWidget_2->setColumnCount(8);
    ui->tableWidget_2->setRowCount(150);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()  << "Название"
                                                                << "Автор"
                                                                << "Доступна"
                                                                << "Комментрай о доступности"
                                                                << "Версия"
                                                                << "Размер"
                                                                << "Кратко"
                                                                << "Дата");
    ui->tableWidget_2->horizontalHeader()->setStyleSheet(
        "  QHeaderView::section {"
        "  border-width: 1px;"
        "  border-style: solid;"
        "  border-color: black"
        "}");
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    ui->tableWidget_2->horizontalHeader()->resizeSection(3, 180);


    // свич табы
    ui->tabWidget->setTabText(0, "Установленные игры");
    ui->tabWidget->setTabText(1, "Репозиторий игр");

}

ManagerWindow::~ManagerWindow()
{
    delete ui;
}
