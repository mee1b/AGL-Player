#include <QApplication>
#include <QThread>
#include <exception>
#include "topwindow.h"

int main(int argc, char *argv[])
{
    try
    {
        // Создаём объект приложения Qt
        QApplication a(argc, argv);

        // Создаём главное окно приложения
        TopWindow tw;
        tw.show();  // показываем окно

        // Запускаем цикл обработки событий Qt
        return a.exec();
    }
    catch (const std::exception& ex)
    {
        QMessageBox::warning(nullptr, "Ошибка!", ex.what());
    }
}
