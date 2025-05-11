#ifndef BASKETBALL_INTERFACE_H
#define BASKETBALL_INTERFACE_H

#include <QtPlugin>

class Basketball_interface
{
public:
    virtual ~Basketball_interface() = default;
    virtual QString printGame();
    virtual QString gameInput(const std::string& playerChoice);
};

#define Basketball_plug_iid "ru.plugBasketball.Basketball_interface"
Q_DECLARE_INTERFACE(Basketball_interface, Basketball_plug_iid)

#endif // BASKETBALL_INTERFACE_H
