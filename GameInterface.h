#pragma once

#include <QString>
#include <QtPlugin>
#include <optional>

//API для игр плагинов
class GameInterface
{
public :
    virtual ~GameInterface() = default;
    //привественное сообщение при включении игры
    virtual QString startMessage() const = 0;
    //обработать команду пользователя и выдать новый текст
    virtual QString gameInput(const QString& playerChoice) = 0;
    //проверить, закончена игра или нет
    virtual bool isOver()const = 0;
    [[nodiscard]] virtual QByteArray saveState(const QString& content) = 0;
    [[nodiscard]] virtual std::optional<QString> loadState(const QByteArray& gameData) = 0;
};

#define GameInterface_iid "ru.AGL-Manager.GameInterface"
Q_DECLARE_INTERFACE(GameInterface, GameInterface_iid)
