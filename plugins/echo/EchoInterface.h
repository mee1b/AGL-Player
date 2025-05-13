#ifndef ECHOINTERFACE_H
#define ECHOINTERFACE_H

#include <QString>
#include <QtPlugin>

class EchoInterface
{
public :
    virtual ~EchoInterface() = default;
    //привественное сообщение при включении игры
    virtual QString startMessage() const = 0;
    //обработать команду пользователя и выдать новый текст
    virtual QString gameInput(const QString& playerChoice) = 0;
    //проверить, закончена игра или нет
    virtual bool isOver()const = 0;
};

#define EchoInterface_iid "ru.echo.EchoInterface"
Q_DECLARE_INTERFACE(EchoInterface, EchoInterface_iid)

#endif // ECHOINTERFACE_H
