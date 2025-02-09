#include "echo.h"

QString PluginEcho::echo(const QString &message)
{
    QString add = "Вы написали: ";
    QString nothing = "Вы ничего не написали!";
    if(message == "") return nothing;
    else return add + message;
}
