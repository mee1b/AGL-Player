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
    ui->switchGame->setTabText(0, "Установленные игры");
    ui->switchGame->setTabText(1, "Репозиторий игр");

    //Hot keys для кнопок
    ui->b_StartGame->setShortcut(QKeySequence(Qt::Key_Return));
    ui->b_LastSaveContin->setShortcut(QKeySequence(Qt::Key_F3));
    ui->b_Delete->setShortcut(QKeySequence(Qt::Key_Delete));
    ui->b_Updata->setShortcut(QKeySequence(Qt::Key_F5));

    connect(ui->b_StartGame, &QPushButton::clicked, this, &Manager::s_StartGame);
    connect(ui->b_LastSaveContin, &QPushButton::clicked, this, &Manager::s_LastSaveContin);
    connect(ui->b_Delete, &QPushButton::clicked, this, &Manager::s_Delete);
    connect(ui->b_Updata, &QPushButton::clicked, this, &Manager::s_Update);
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
    if(event->key() == Qt::Key_Escape)
    {
        emit closeManagerWindow();
        close();
    }
}

void Manager::s_StartGame()
{
    QListWidgetItem* item = ui->plugList->currentItem();
    emit ui->plugList->itemActivated(item);
    close();
}


void Manager::s_Delete()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}



void Manager::s_Update()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}


void Manager::s_LastSaveContin()
{
    QMessageBox::information(this, tr("Error!"), tr("Не реализовано!"));
}

void Manager::closeEvent(QCloseEvent *event)
{
    emit closeManagerWindow();
    QWidget::closeEvent(event);
}

