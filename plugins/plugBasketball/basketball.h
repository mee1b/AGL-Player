#ifndef BASKETBALL_H
#define BASKETBALL_H

#include <QObject>
#include "basketball_interface.h"

class Basketball : public QObject, public Basketball_interface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID Basketball_plug_iid FILE "plugBasketball.json")
    Q_INTERFACES(Basketball_interface)

public:
    explicit Basketball(QObject *parent = nullptr);
    QString printGame() override;
    QString gameInput(const std::string& playerChoice) override;
    QString message{};
};

#endif // BASKETBALL_H
