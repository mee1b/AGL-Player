#include "managerwindow.h"
#include "mainwindow.h"

#include <Windows.h>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Application Example");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open");
    parser.process(a);

    MainWindow mw;
    mw.show();
    ManagerWindow w;
    w.show();
    return a.exec();


}
/*Основная функция использует QCommandLineParser для проверки,
 * был ли передан приложению какой-либо аргумент файла, и загружает его с помощью MainWindow::loadFile().*/
