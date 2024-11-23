#include "headerwindow.h"
#include "ui_headerwindow.h"

headerwindow::headerwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::headerwindow)
{
    ui->setupUi(this);
    setWindowTitle("AGL-Manager");

    ui->toolButton->setStyleSheet("QToolButton{background: transparent;}");
    ui->toolButton_2->setStyleSheet("QToolButton{background: transparent;}");
    ui->toolButton_3->setStyleSheet("QToolButton{background: transparent;}");
    ui->toolButton_4->setStyleSheet("QToolButton{background: transparent;}");

    ui->textEdit->setFontPointSize(14);

}

headerwindow::~headerwindow()
{
    delete ui;
}



void headerwindow::on_toolButton_customContextMenuRequested(const QPoint &pos)
{

}

