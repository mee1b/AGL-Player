#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("AGL-Manager");

    ui->menu->addAction("Менеджер игр");
    ui->menu->addAction("Добавить игру в библиотеку");
    ui->menu->addAction("Открыть сохранение");
    ui->menu->addAction("Открыть сохранение без перезагрузки библиотеки");
    ui->menu->addAction("Сохранить игру");
    ui->menu->addAction("Начать игру заново");
    ui->menu->addSeparator();
    ui->menu->addAction("Сбросить все настройки");
    ui->menu->addAction("Выход");

}

MainWindow::~MainWindow()
{
    delete ui;
}
