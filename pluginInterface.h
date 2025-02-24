#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>
#include <QWidget>

class pluginInterface : public QObject
{
public:
    virtual ~pluginInterface(){}
    virtual QString getNamePlugin() = 0;
};

Q_DECLARE_INTERFACE(pluginInterface, "ru.AGL-Manager.Application.Interface")

#endif // PLUGININTERFACE_H
