#include "managerwindow.h"
#include "headerwindow.h"

#include <Windows.h>
#include <QApplication>


int main(int argc, char *argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    QApplication a(argc, argv);
    headerwindow h;
    h.show();
    ManagerWindow w;
    w.show();
    return a.exec();


}
