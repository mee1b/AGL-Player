#include "basketball.h"
#include "namespaces.h"
#include "enums.h"
#include <QTest>


Basketball::Basketball(QObject *parent)
    : QObject(parent), distrib(1, 100)
{
    static std::mt19937::result_type seed = static_cast<unsigned int>(time(NULL));
    gen.seed(seed);
}

QString Basketball::startMessage() const
{
    QString buff = menu::WELCOME + menu::rules;
    return buff;
}

QString Basketball::gameInput(const QString &playerChoice)
{
    outputMessage.clear();

    if(playerChoice.toLower() == "exit")
    {
        gameOver = true;
        return end;
    }

        switch(currentStep)
        {
        case Step::Start:
            if(playerChoice.toLower() == "start")
            {
                player.name = history::PLAYER_TEAM_NAME;
                opponent.name = history::STANDART_OPPONENT_NAME;
                player.teamSpirit = engine::ZERO;
                opponent.teamSpiritOpponent = engine::ZERO;
                player.score = engine::ZERO;
                opponent.score = engine::ZERO;
                currentStep = Step::DefenseChoice;
                outputMessage = menu::rulesDefense;
            }
            else
            {
                outputMessage = menu::repeatStart;
            }
            break;

        case Step::DefenseChoice:
        {
            int defenseChoice = playerChoice.toInt();
            if(defenseChoice < 1 || defenseChoice > 4)
            {
                outputMessage = menu::REPEAT;
                break;
            }
            player.defense = defenseChoice;
            choiceDefenseOpponent(opponent.defense);

            outputMessage += "Введите любой символ для продолжения...";
            currentStep = Step::PlayerTurn;
        }
        break;

        case Step::PlayerTurn:
        {
            bool scored = false;

            if(awaitingShotInput)
            {
                outputMessage = menu::rulesShot;
                awaitingShotInput = false;
                break;
            }

            player.shot = playerChoice.toInt();
            if(player.shot < 1 || player.shot > 4)
            {
                outputMessage = menu::REPEAT;
                break;
            }

            outputMessage += "\n" + attackShotDescription(player.shot, player.teamSpirit);

            QString correctShot;
            switch(player.shot)
            {
            case 1:
                correctShot = "Трехочковый бросок";
                break;
            case 2:
                correctShot = "Двухочковый бросок";
                break;
            case 3:
                correctShot = "Лэй-апп";
                break;
            case 4:
                correctShot = "Комбинация";
                break;
            }

            outputMessage = QString("Вы выбрали %1.\n").arg(correctShot);

            outputMessage += playerAttack(player, opponent, scored);

            if(player.score >= 20)
            {
                outputMessage += "\nПоздравляем! Вы победили!!!";
                outputMessage += "\n\nВведите любой символ для продолжения...";
                currentStep = Step::End;
                break;

            }

            if(scored)
            {
                outputMessage += QString("\nСчет на табло: %1 - %2").arg(player.score).arg(opponent.score);
                outputMessage += "\n\nВведите любой символ для продолжения...";
                currentStep = Step::OpponentTurn;
            }
            else
            {
                currentStep = Step::OpponentTurn;
                awaitingShotInput = true;
                break;
            }
            break;
        }
        case Step::OpponentTurn:
        {
            bool scoredOpponent = false;

            outputMessage += QString("%1 в атаке!\n").arg(opponent.name);
            outputMessage += opponentAttack(player, opponent, scoredOpponent);

            if(scoredOpponent)
            {
                outputMessage += QString("\nСчет на табло: %1 - %2").arg(player.score).arg(opponent.score);
            }

            if(player.score >= 20)
            {
                outputMessage += "\nПоздравляем! Вы победили!!!";
                currentStep = Step::End;
                outputMessage += "\n\nВведите любой символ для продолжения...";
                break;

            }
            else if(opponent.score >= 20)
            {
                outputMessage += "\nПобеда была близка, не расстраивайся!";
                currentStep = Step::End;
                outputMessage += "\n\nВведите любой символ для продолжения...";
                break;
            }
            else
            {
                currentStep = Step::PlayerTurn;
                awaitingShotInput = true;
            }
            break;
        }
        case Step::End:
            outputMessage = QString("Счет на табло: %1 - %2").arg(player.score).arg(opponent.score) + "\nИгра завершена.\nВведите 'start', чтобы сыграть снова, или 'exit', чтобы выйти.";
            if (playerChoice.toLower() == "start")
            {
                currentStep = Step::Start;
            }
            else if(playerChoice.toLower() == "exit")
            {
                gameOver = true;
            }
            break;
    }

    return outputMessage;
}

bool Basketball::isOver() const
{
    return gameOver;
}

void Basketball::choiceDefenseOpponent(int& defense)
{
    int choiceDefense = distrib(gen);
    if (choiceDefense >= history::PROBALITY_WIN)
    {
        defense = static_cast<int>(defense::PERSONAL_DEFENSE);
    }
    else
    {
        defense = static_cast<int>(defense::ZONE_DEFENSE);
    }
}

void Basketball::probabilityHitPlayer(int& hit, const int& teamSpirit)
{
    hit = distrib(gen);
}

void Basketball::probabilityHitOpponent(int& hit, const int& teamSpiritOpponent)
{
    hit = distrib(gen);
}

void Basketball::probalityStealOpponentOnPlayer(bool& steal)
{
    steal = false;
    int probalitySteal = distrib(gen);
    if (probalitySteal > engine::GOOD_STEAL_OPPONENT_ON_PLAYER)
    {
        steal = true;
    }
}

void Basketball::probalityBlockOpponentOnPlayer(bool& block)
{
    block = false;
    int probalityBlock = distrib(gen);
    if (probalityBlock > engine::GOOD_BLOCK_OPPONENT_ON_PLAYER)
    {
        block = true;
    }
}

QString Basketball::attackShotDescription(const int& shot, const int& teamSpirit)
{
    QString outputText{""};

    switch(menu::hint)
    {
    case static_cast<int>(playerHints::EXPERT):
        outputText += attack::SHOT_CHOICE;
        break;
    case static_cast<int>(playerHints::AMATEUR):
        outputText += attack::SHOT_CHOICE_AND_HINT;
    }

    if(shot == static_cast<int>(shots::DIRTY_SHOT) && teamSpirit <= static_cast<int>(spirit::DIRTY_SPIRIT))
    {
        outputText += "\nГрязная игра разрешена.";
        return outputText;
    }

    if(shot == static_cast<int>(shots::HAND_GOD_SHOT) && teamSpirit <= static_cast<int>(spirit::GOD_SPIRIT))
    {
        outputText += "\nВы используете руку бога!";
        return outputText;
    }

    if(shot < static_cast<int>(shots::THREE_POINT_SHOT) || shot > static_cast<int>(shots::COMBINATIONT_SHOT))
    {
        outputText += "\n" + attack::UNKNOW_TACTICS;
        return outputText;
    }

    return outputText;
}

QString Basketball::playerAttack(Player& player, Opponent& opponent, bool& scored)
{
    QString result;
    int hit{};

    probabilityHitPlayer(hit, player.teamSpirit);

    probalityBlockOpponentOnPlayer(blocked);
    if(blocked)
    {
        result += "Соперник блокирует бросок!\n\nВведите любой символ для продолжения...";
        scored = false;
        return result;
    }

    probalityStealOpponentOnPlayer(stealed);
    if(stealed)
    {
        result += "Соперник перехватывает мяч!\n\nВведите любой символ для продолжения...";
        scored = false;
        return result;
    }

    if(player.shot == 1 && hit >= 50)
    {
        player.score += 3;
        result += "Мяч в корзине!";
        scored = true;
        return result;
    }
    else if(player.shot != 1 && hit > 40)
    {
        player.score += 2;
        result += "Мяч в корзине!";
        scored = true;
        return result;
    }
    else scored = false;
    result += attack::LOSE_SHOT;

    return result;
}

void Basketball::switchDefenseOpponent(const Player& player, Opponent& opponent)
{
    if ((opponent.defense == static_cast<int>(defense::PRESSING)) && (player.score - opponent.score < opponent.DIRTY_DEEP))
    {
        choiceDefenseOpponent(opponent.defense);
    }
    if (player.score - opponent.score >= opponent.DIRTY_DEEP)
    {
        opponent.defense = static_cast<int>(defense::PRESSING);
    }
    else if (opponent.score - player.score >= opponent.DIRTY_DEEP)
    {
        opponent.defense = static_cast<int>(defense::NONE_DEFENSE);
    }
    switch (player.shot)
    {
    case static_cast<int>(shots::THREE_POINT_SHOT):
    case static_cast<int>(shots::COMBINATIONT_SHOT):
        if (engine::countZoneDefense == engine::SWITCH_DEFENSE)
        {
            opponent.defense = static_cast<int>(defense::ZONE_DEFENSE);
            engine::countZoneDefense = engine::ZERO;
            break;
        }
        engine::countZoneDefense += engine::ONE_UP;
        engine::countPersonalDefense = engine::ZERO;
        break;

    case static_cast<int>(shots::MEDIUM_SHOT):
    case static_cast<int>(shots::LAY_UP_SHOT):
        if (engine::countPersonalDefense == engine::SWITCH_DEFENSE)
        {
            opponent.defense = static_cast<int>(defense::PERSONAL_DEFENSE);
            engine::countPersonalDefense = engine::ZERO;
            break;
        }
        engine::countPersonalDefense += engine::ONE_UP;
        engine::countZoneDefense = engine::ZERO;
        break;
    }
}

QString Basketball::opponentAttack(Player &player, Opponent &opponent, bool& scoredOpponent)
{
    QString result;
    int hit{};

    probabilityHitOpponent(hit, opponent.teamSpiritOpponent);

    if(hit >= 55)
    {
        opponent.score += 2;
        result += "Мяч в корзине!\n\nВведите любой символ для продолжения...";
        scoredOpponent = true;
        return result;
    }
    result += "Это было близко, но мимо...\n\nВведите любой символ для продолжения...";
    return result;
}

