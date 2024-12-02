#include "managerwindow.h"
#include "./ui_managerwindow.h"
#include <QShortcut>
#include <QMessageBox>


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
                                                                << "Комментарий о доступности"
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
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < tableName.size(); ++i)
    {
        ui->tableWidget->setItem(0, i, new QTableWidgetItem(tableName[i]));
        ui->tableWidget->setItem(1, i, new QTableWidgetItem(tableName_2[i]));
    }
    for (int i = 0; i < 8; ++i)
    {
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }

     //таблица 2
    ui->tableWidget_2->setColumnCount(8);
    ui->tableWidget_2->setRowCount(150);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()  << "Название"
                                                                << "Автор"
                                                                << "Доступна"
                                                                << "Комментарий о доступности"
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
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < tableName.size(); ++i)
    {
        ui->tableWidget_2->setItem(0, i, new QTableWidgetItem(tableName[i]));
        ui->tableWidget_2->setItem(1, i, new QTableWidgetItem(tableName_2[i]));
        ui->tableWidget_2->resizeColumnsToContents();
    }



    // свич табы
    ui->tabWidget->setTabText(0, "Установленные игры");
    ui->tabWidget->setTabText(1, "Репозиторий игр");

    //Hot keys для кнопок
    ui->pushButton->setShortcut(QKeySequence("F1"));
    ui->pushButton_2->setShortcut(QKeySequence("F3"));
    ui->pushButton_3->setShortcut(QKeySequence("DEL"));
    ui->pushButton_4->setShortcut(QKeySequence("F5"));


}

ManagerWindow::~ManagerWindow()
{
    delete ui;
}



void ManagerWindow::on_pushButton_clicked()
{
    QMessageBox::warning(this, tr("Error!"), tr("Не реализовано!"));
}


void ManagerWindow::on_pushButton_2_clicked()
{
    QMessageBox::warning(this, tr("Error!"), tr("Не реализовано!"));
}


void ManagerWindow::on_pushButton_3_clicked()
{
    QMessageBox::warning(this, tr("Error!"), tr("Не реализовано!"));
}


void ManagerWindow::on_pushButton_4_clicked()
{
    QMessageBox::warning(this, tr("Error!"), tr("Не реализовано!"));
}

