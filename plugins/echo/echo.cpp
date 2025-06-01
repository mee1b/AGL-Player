#include "echo.h"

QString PluginEcho::gameInput(const QString &playerChoice)
{
    checkQuit = playerChoice;
    if(playerChoice == "") return nothing;
    else return add + playerChoice;
}

QString PluginEcho::startMessage() const
{
    return welcome;
}

bool PluginEcho::isOver() const
{
    return quit.toLower().trimmed() == checkQuit.toLower().trimmed();
}
