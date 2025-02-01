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
    QString echo(const QString& message) override;
};

#endif // ECHO_H
