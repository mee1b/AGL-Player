#include "manager.h"
#include "ui_manager.h"

Manager::Manager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Manager)
{
    ui->setupUi(this);
    setWindowTitle("Менеджер игр");
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint);
    setWindowFlag(Qt::WindowCloseButtonHint, true);


    // свич табы
    ui->tabWidget->setTabText(0, "Установленные игры");
    ui->tabWidget->setTabText(1, "Репозиторий игр");

    //Hot keys для кнопок
    ui->pushButton->setShortcut(QKeySequence(Qt::Key_Return));
    ui->pushButton_4->setShortcut(QKeySequence(Qt::Key_F3));
    ui->pushButton_2->setShortcut(QKeySequence(Qt::Key_Delete));
    ui->pushButton_3->setShortcut(QKeySequence(Qt::Key_F5));
}

Manager::~Manager()
{
    delete ui;
}

void Manager::updateLists()
{
    for(const auto& plugin : namePlugin)
    {
        ui->plugList->addItem(plugin);
        ui->plugList_2->addItem(plugin);
    }

}

QListWidget *Manager::getPlugList() const
{
    return ui->plugList;
}

void Manager::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape) close();
}

void Manager::on_pushButton_clicked()
{
    QListWidgetItem* item = ui->plugList->currentItem();
    emit ui->plugList->itemActivated(item);
    close();
}


void Manager::on_pushButton_4_clicked()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}



void Manager::on_pushButton_3_clicked()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}


void Manager::on_pushButton_2_clicked()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}

