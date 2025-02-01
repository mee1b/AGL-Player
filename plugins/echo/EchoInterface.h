#ifndef ECHOINTERFACE_H
#define ECHOINTERFACE_H

#include <QString>
#include <QtPlugin>

class EchoInterface
{
public :
    virtual ~EchoInterface() = default;
    virtual QString echo(const QString& message) = 0;
};

#define EchoInterface_iid "ru.echo.EchoInterface"
Q_DECLARE_INTERFACE(EchoInterface, EchoInterface_iid)

#endif // ECHOINTERFACE_H
