#include "basketball.h"

Basketball::Basketball(QObject *parent)
    : QObject(parent)
{}

QString Basketball::printGame()
{
    return message;
}

QString Basketball::gameInput(const std::string& playerChoice)
{
    QString buf = QString::fromStdString(playerChoice);
    return buf;
}

