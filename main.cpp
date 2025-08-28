#include <QApplication>
#include <QThread>

#include "topwindow.h"

int main(int argc, char *argv[])
{
    // Создаём объект приложения Qt
    QApplication a(argc, argv);

    // Создаём главное окно приложения
    TopWindow tw;
    tw.show();  // показываем окно

    // Запускаем цикл обработки событий Qt
    return a.exec();
}
