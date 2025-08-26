#include <Windows.h>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QListWidget>

#include "topwindow.h"

int main(int argc, char *argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    QApplication a(argc, argv);

    TopWindow tw;
    tw.show();

    return a.exec();
}

