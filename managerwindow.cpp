#include "managerwindow.h"
#include "./ui_managerwindow.h"
#include <QShortcut>
#include <QMessageBox>


ManagerWindow::ManagerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ManagerWindow)
{
    ui->setupUi(this);
    this->statusBar()->setSizeGripEnabled(false);
    setFixedSize(700, 400);
    setWindowTitle("Менеджер игр");
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint);
    setWindowFlag(Qt::WindowCloseButtonHint, true);



    //таблица 1
    ui->listWidget_2->addItem(tableName);
    ui->listWidget_2->addItem(tableName_2);

     //таблица 2

    ui->listWidget->addItem(tableName);
    ui->listWidget->addItem(tableName_2);



    // свич табы
    ui->tabWidget->setTabText(0, "Установленные игры");
    ui->tabWidget->setTabText(1, "Репозиторий игр");

    //Hot keys для кнопок
    ui->pushButton->setShortcut(QKeySequence(Qt::Key_Return));
    ui->pushButton_4->setShortcut(QKeySequence(Qt::Key_F3));
    ui->pushButton_2->setShortcut(QKeySequence(Qt::Key_Delete));
    ui->pushButton_3->setShortcut(QKeySequence(Qt::Key_F5));


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

