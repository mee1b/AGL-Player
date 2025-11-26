#pragma once

#include <QObject>
#include "../../GameInterface.h"
#include "structs.h"
#include "enums.h"
#include <random>

class Basketball : public QObject, public GameInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID GameInterface_iid FILE "Basketball.json")
    Q_INTERFACES(GameInterface)

    std::mt19937 gen;
    std::uniform_int_distribution<unsigned int> distrib;

public:
    explicit Basketball(QObject *parent = nullptr);
    //привественное сообщение при включении игры
    QString startMessage() const override;
    //обработать команду пользователя и выдать новый текст
    QString gameInput(const QString& playerChoice) override;
    //проверить, закончена игра или нет
    bool isOver()const override;
    [[nodiscard]] QByteArray saveState(const QString& content) override;
    [[nodiscard]] std::optional<QString> loadState(const QByteArray& gameData) override;
    int getUndoAttempts() const noexcept override;

private:
    bool gameOver = false;
    const int undoAttempts = 3;
    bool blocked = false;
    bool stealed = false;
    Opponent opponent;
    Player player;
    QString outputMessage;
    QString end = "Игра завершена.";

    std::map<defense, QString> defenseName =
        {
            {defense::PRESSING, "Прессинг"},
            {defense::PERSONAL_DEFENSE, "Персональная защита"},
            {defense::ZONE_DEFENSE, "Зонная защита"},
            {defense::NONE_DEFENSE, "Нет защиты"}
        };

    enum class Step
    {
        Start,
        DefenseChoice,
        PlayerRulesShot,
        PlayerInputShot,
        OpponentTurn,
        End
    };

    Step currentStep = Step::Start;
    int currentShot = 0;
    bool awaitingShotInput = true;

    void choiceDefenseOpponent(int& defense);
    void probabilityHitPlayer(int& hit, const int& teamSpirit);
    void probabilityHitOpponent(int& hit, const int& teamSpiritOpponent);
    void probalityStealOpponentOnPlayer(bool& steal);
    void probalityBlockOpponentOnPlayer(bool& block);
    QString attackShotDescription(const int& shot, const int& teamSpirit);
    QString playerAttack(Player& player, Opponent& opponent, bool& scored);
    void switchDefenseOpponent(const Player& player, Opponent& opponent);
    QString opponentAttack(Player& player, Opponent& opponent, bool& scoredOpponent);
    QString autoStep();
    QString showDefensePlayer(const Player& player);
    QString showDefenseOpponent(const Opponent& opponent);
    bool rebound();
    bool choiceOpponentShot();
    void checkTeamSpirit(int& teamSpirit);
};
