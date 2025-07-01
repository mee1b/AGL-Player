#pragma once

#include <QString>

//Оболочка над talk DLL, работает только если реально talk подключен
//см. пример использования тут: https://github.com/dkager/tolk/blob/master/examples/c%2B%2B/ConsoleApp.cpp
class TalkWrap
{
public:
    TalkWrap();
    QString currentReader() const; //текущий ридер
    void output(const QString& text, bool interrupt = false); //вывести текст, по флагу можно прервать предыдущий вывод речи
    ~TalkWrap();
private:
    const QString reader_;
    const bool valid_;
};
