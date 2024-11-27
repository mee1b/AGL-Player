#include "managerwindow.h"
#include "mainwindow.h"

#include <Windows.h>
#include <QApplication>


int main(int argc, char *argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    QApplication a(argc, argv);
    MainWindow h;
    h.show();
    ManagerWindow w;
    w.show();
    return a.exec();


}
