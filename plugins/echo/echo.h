#pragma once

#include <QObject>
#include "../../GameInterface.h"

class PluginEcho : public QObject, GameInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID GameInterface_iid FILE "echo.json")
    Q_INTERFACES(GameInterface)

public:
    QString gameInput(const QString& playerChoice) override;
    QString startMessage() const override;
    bool isOver() const override;

private:
    QString add = "Вы написали: ";
    QString nothing = "Вы ничего не написали!";
    QString welcome = "Добро пожаловать в игру \"ЭХО\"\nДля завершения игры введите \"выход\"";
    QString quit = "выход";
    QString checkQuit{};
};
