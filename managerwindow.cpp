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
    setWindowFlags(Qt::Window | Qt::WindowTitleHint);
    setWindowFlag(Qt::WindowCloseButtonHint, true);


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
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < tableName.size(); ++i)
    {
        ui->tableWidget->setItem(0, i, new QTableWidgetItem(tableName[i]));
        ui->tableWidget->setItem(1, i, new QTableWidgetItem(tableName_2[i]));
        ui->tableWidget->resizeColumnsToContents();
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
    ui->pushButton->setShortcut(QKeySequence(Qt::Key_Return));
    ui->pushButton_2->setShortcut(QKeySequence(Qt::Key_F3));
    ui->pushButton_3->setShortcut(QKeySequence(Qt::Key_Delete));
    ui->pushButton_4->setShortcut(QKeySequence(Qt::Key_F5));


}

ManagerWindow::~ManagerWindow()
{
    delete ui;
}



void ManagerWindow::on_pushButton_clicked()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}


void ManagerWindow::on_pushButton_2_clicked()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}


void ManagerWindow::on_pushButton_3_clicked()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}


void ManagerWindow::on_pushButton_4_clicked()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}

