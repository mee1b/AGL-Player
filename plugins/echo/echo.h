#ifndef ECHO_H
#define ECHO_H

#include <QObject>
#include "EchoInterface.h"

class PluginEcho : public QObject, EchoInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID EchoInterface_iid FILE "echo.json")
    Q_INTERFACES(EchoInterface)

public:
    QString gameInput(const QString& playerChoice) override;
    QString startMessage() const override;
    bool isOver() const override;

private:
    QString add = ">Вы написали: ";
    QString nothing = ">Вы ничего не написали!";
    QString welcome = ">Добро пожаловать в игру \"ЭХО\"\nДля завершения игры введите \"exit\"";
    QString quit = "exit";
    QString checkQuit{};
};

#endif // ECHO_H
