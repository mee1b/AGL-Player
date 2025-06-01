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
    return menu::WELCOME;
}

QString Basketball::gameInput(const QString &playerChoice)
{
    outputMessage.clear();

    QString input = playerChoice.toLower().trimmed();
    if (input == "выход")
    {
        gameOver = true;
        return end;
    }
        switch (currentStep)
        {
        case Step::Start:
            if (input == "начать")
            {
                player.name = history::PLAYER_TEAM_NAME;
                opponent.name = history::STANDART_OPPONENT_NAME;
                player.teamSpirit = engine::ZERO;
                opponent.teamSpiritOpponent = engine::ZERO;
                player.score = engine::ZERO;
                opponent.score = engine::ZERO;
                currentStep = Step::DefenseChoice;
                input.clear();
                return menu::rulesDefense;
            }
            else
            {
                return menu::repeatStart;
            }

        case Step::DefenseChoice:
        {
            int defenseChoice = input.toInt();
            if (defenseChoice >= 1 && defenseChoice <= 4) {
                player.defense = defenseChoice;
                choiceDefenseOpponent(opponent.defense);
                currentStep = Step::PlayerRulesShot;
                input.clear();
                break;
            }
            else
            {
                return menu::REPEAT;
            }
        }

        case Step::PlayerInputShot:
        {
            int shot = input.toInt();
            if (shot < 1 || shot > 4)
            {
                return menu::REPEAT;
            }

            player.shot = shot;
            QString correctShot;

            switch (shot)
            {
            case 1: correctShot = "Трехочковый бросок"; break;
            case 2: correctShot = "Двухочковый бросок"; break;
            case 3: correctShot = "Лэй-апп"; break;
            case 4: correctShot = "Комбинация"; break;
            }

            outputMessage = QString("Вы выбрали %1.\n").arg(correctShot);
            outputMessage += "\n" + attackShotDescription(shot, player.teamSpirit);

            bool scored = false;
            outputMessage += playerAttack(player, opponent, scored);

            if (player.score >= 20)
            {
                outputMessage = "Поздравляем! Вы победили!!!" + QString("Счет на табло: %1 - %2").arg(player.score).arg(opponent.score) +
                                "\nИгра завершена.\nВведите 'начать', чтобы сыграть снова, или 'выход', чтобы выйти.";;
                currentStep = Step::End;
                break;
            }

            if (scored)
            {
                outputMessage += QString("\nСчет на табло: %1 - %2").arg(player.score).arg(opponent.score);
            }

            currentStep = Step::OpponentTurn;
            input.clear();
            break;
        }

        case Step::End:
            if (input == "начать")
            {
                currentStep = Step::Start;
                input.clear();
                break;
            }
            else if (input == "выход")
            {
                gameOver = true;
                break;
            }
            else
            {
                outputMessage = "Игра завершена.\nВведите 'начать', чтобы сыграть снова, или 'выход', чтобы выйти.";
                return outputMessage.trimmed();
            }
        }

        QString autoMessage = outputMessage;
        while(true)
        {
            QString message = autoStep();
            if(message.isEmpty())
                break;
            autoMessage += '\n' + message;
        }
        return autoMessage.trimmed();
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
        result += "Соперник блокирует бросок!";
        scored = false;
        return result;
    }

    probalityStealOpponentOnPlayer(stealed);
    if(stealed)
    {
        result += "Соперник перехватывает мяч!";
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
        result += "Мяч в корзине!";
        scoredOpponent = true;
        return result;
    }
    result += "Это было близко, но мимо...";
    return result;
}

QString Basketball::autoStep()
{
    QString result;
    switch(currentStep)
    {
    case Step::PlayerRulesShot:
        result += menu::rulesShot;
        currentStep = Step::PlayerInputShot;
        break;
    case Step::OpponentTurn:
    {
        bool scoredOpponent = false;
        result = QString("%1 в атаке!\n").arg(opponent.name);
        result += opponentAttack(player, opponent, scoredOpponent);

        if (scoredOpponent)
        {
            result += QString("\nСчет на табло: %1 - %2").arg(player.score).arg(opponent.score);
        }

        if (player.score >= 20)
        {
            result += "Поздравляем! Вы победили!!!" + QString("Счет на табло: %1 - %2").arg(player.score).arg(opponent.score) +
                            "\nИгра завершена.\nВведите 'начать', чтобы сыграть снова, или 'выход', чтобы выйти.";
            currentStep = Step::End;
            break;
        }
        else if (opponent.score >= 20)
        {
            result += "Победа была близка! Не расстраивайся!" + QString("Счет на табло: %1 - %2").arg(player.score).arg(opponent.score) +
                            "\nИгра завершена.\nВведите 'начать', чтобы сыграть снова, или 'выход', чтобы выйти.";
            currentStep = Step::End;
            break;
        }
        else
        {
            currentStep = Step::PlayerRulesShot;
        }
    }
    default:
        break;
    }
    return result;
}

