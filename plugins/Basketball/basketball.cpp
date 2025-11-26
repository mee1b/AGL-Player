#include "basketball.h"
#include "namespaces.h"
#include <map>
#include <QSaveFile>
#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>
#include <optional>


Basketball::Basketball(QObject *parent)
    : QObject(parent), distrib(1, 100)
{
    static std::mt19937::result_type seed = static_cast<unsigned int>(time(NULL));
    gen.seed(seed);
}

QByteArray Basketball::saveState(const QString& content)
{

    // -------------------------------
    // Формируем JSON объект для сохранения данных
    // -------------------------------
    QJsonObject obj;
    obj["step"] = static_cast<int>(currentStep);   // Сохраняем текущий шаг игры, кастим enum к int
    obj["player-score"] = player.score;           // Сохраняем счет игрока
    obj["opponent-score"] = opponent.score;       // Сохраняем счет противника
    obj["player-spirit"] = player.teamSpirit;     // Сохраняем командный дух игрока
    obj["opponent-spirit"] = opponent.teamSpiritOpponent; // Сохраняем командный дух противника
    obj["player-defense"] = player.defense;     // Сохраняем защиту игрока
    obj["opponent-defense"] = opponent.defense; // Сохраняем защиту противника
    obj["player-name"] = player.name;             // Сохраняем название команды игрока
    obj["opponent-name"] = opponent.name;         // Сохраняем название команды противника
    obj["ui-content"] = content;                  // Текст на экране, на момент сохранения

    // -------------------------------
    // Превращаем JSON объект в QJsonDocument
    // -------------------------------
    QJsonDocument doc(obj);
    QByteArray gameData = doc.toJson();                // Преобразуем документ в последовательность байтов для записи в файл

    return gameData;                                  // Состояние успешно сохранено
}


std::optional<QString> Basketball::loadState(const QByteArray& gameData)
{
    QJsonParseError parseError;                       // Структура для хранения ошибки парсинга
    QJsonDocument doc = QJsonDocument::fromJson(gameData, &parseError);
    if(parseError.error != QJsonParseError::NoError)  // Если произошла ошибка при парсинге
    {
        return std::nullopt;
    }

    // -------------------------------
    // Получаем объект JSON
    // -------------------------------
    if(!doc.isObject())
    {
        return std::nullopt;
    }
    QJsonObject obj = doc.object();

    // -------------------------------
    // Читаем и проверяем данные
    // -------------------------------
    if(obj.contains("step") && obj["step"].isDouble())      // В Qt числа в JSON всегда double
        currentStep = static_cast<Step>(obj["step"].toDouble());
    else
    {
        return std::nullopt;
    }

    if(obj.contains("player-score") && obj["player-score"].isDouble())
        player.score = static_cast<int>(obj["player-score"].toDouble());
    else
    {
        return std::nullopt;
    }

    if(obj.contains("opponent-score") && obj["opponent-score"].isDouble())
        opponent.score = static_cast<int>(obj["opponent-score"].toDouble());
    else
    {
        return std::nullopt;
    }

    if(obj.contains("player-spirit") && obj["player-spirit"].isDouble())
    {
        player.teamSpirit = static_cast<int>(obj["player-spirit"].toDouble());
    }
    else
    {
        return std::nullopt;
    }


    if(obj.contains("opponent-spirit") && obj["opponent-spirit"].isDouble())
    {
        opponent.teamSpiritOpponent = static_cast<int>(obj["opponent-spirit"].toDouble());
    }
    else
    {
        return std::nullopt;
    }

    if(obj.contains("player-defense") && obj["player-defense"].isDouble())
    {
        player.defense = static_cast<int>(obj["player-defense"].toDouble());
    }
    else
    {
        return std::nullopt;
    }

    if(obj.contains("opponent-defense") && obj["opponent-defense"].isDouble())
    {
        opponent.defense = static_cast<int>(obj["opponent-defense"].toDouble());
    }
    else
    {
        return std::nullopt;
    }

    if(obj.contains("player-name") && obj["player-name"].isString())
    {
        player.name = obj["player-name"].toString();
    }
    else
    {
        return std::nullopt;
    }

    if(obj.contains("opponent-name") && obj["opponent-name"].isString())
    {
        opponent.name = obj["opponent-name"].toString();
    }
    else
    {
        return std::nullopt;
    }

    if(obj.contains("ui-content") && obj["ui-content"].isString())
    {
        return obj["ui-content"].toString(); // Успешная загрузка
    }

    return std::nullopt;
}

int Basketball::getUndoAttempts() const noexcept
{
    return undoAttempts;
}

QString Basketball::startMessage() const
{
    return menu::WELCOME;
}

QString Basketball::gameInput(const QString &playerChoice)
{
    outputMessage.clear();
    gameOver = false;

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
                outputMessage = menu::RULES_IN_GAME;
                player.name = history::PLAYER_TEAM_NAME;
                opponent.name = history::STANDART_OPPONENT_NAME;
                player.teamSpirit = engine::ZERO;
                opponent.teamSpiritOpponent = engine::ZERO;
                player.score = engine::ZERO;
                opponent.score = engine::ZERO;
                currentStep = Step::DefenseChoice;
                input.clear();
                outputMessage += menu::rulesDefense;
                return outputMessage;
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
            if (shot < 0 || shot > 4)
            {
                return menu::REPEAT;
            }

            player.shot = shot;
            QString correctShot;

            if(player.shot == 0)
            {
                outputMessage = menu::rulesDefense;
                currentStep = Step::DefenseChoice;
                return outputMessage;
            }

            switch (shot)
            {
            case 1: correctShot = "Трехочковый бросок"; break;
            case 2: correctShot = "Двухочковый бросок"; break;
            case 3: correctShot = "Лэй-апп"; break;
            case 4: correctShot = "Комбинация"; break;
            }
            outputMessage = QString("Вы выбрали %1.\n").arg(correctShot);
            outputMessage += showDefensePlayer(player);
            outputMessage += "\n" + attackShotDescription(shot, player.teamSpirit);

            bool scored = false;
            outputMessage += playerAttack(player, opponent, scored);

            if (player.score >= 20)
            {
                outputMessage = "Поздравляем! Вы победили!!!" + QString("Счет на табло: %1 - %2").arg(player.score).arg(opponent.score) +
                                "\nИгра завершена.\nВведите 'повтор', чтобы сыграть снова, или 'выход', чтобы выйти.";;
                currentStep = Step::End;
                break;
            }

            if (scored)
            {
                outputMessage += QString("\nСчет на табло: %1 - %2\nВаш командный дух: %3").arg(player.score).arg(opponent.score).arg(player.teamSpirit);
            }
            else
            {
                if(rebound() && (stealed == false) && (blocked == false))
                {
                    outputMessage += QString("Ваша команда подобрала мяч!\nВаш командный дух: %1").arg(player.teamSpirit);
                    currentStep = Step::PlayerRulesShot;
                    break;
                }
                else if ((stealed == false) && (blocked == false))
                {
                    outputMessage += QString("Подбор за %1!\nВаш командный дух: %2").arg(opponent.name).arg(player.teamSpirit);
                }
            }

            currentStep = Step::OpponentTurn;
            input.clear();
            break;
        }

        case Step::End:
            if (input.toLower().trimmed() == "повтор")
            {
                currentStep = Step::Start;
                input.clear();
                return Basketball::startMessage();
            }
            else if (input.toLower().trimmed() == "выход")
            {
                gameOver = true;
                break;
            }
            else
            {
                outputMessage = "Игра завершена.\nВведите 'повтор', чтобы сыграть снова, или 'выход', чтобы выйти.";
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
    hit = distrib(gen) + (teamSpirit / 2);
}

void Basketball::probabilityHitOpponent(int& hit, const int& teamSpiritOpponent)
{
    hit = distrib(gen) + (teamSpiritOpponent / 2);
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
        player.teamSpirit += 5;
        checkTeamSpirit(player.teamSpirit);
        scored = true;
        return result;
    }
    else if(player.shot != 1 && hit > 40)
    {
        player.score += 2;
        result += "Мяч в корзине!";
        player.teamSpirit += 5;
        checkTeamSpirit(player.teamSpirit);
        scored = true;
        return result;
    }
    else
    {
        scored = false;
        player.teamSpirit -= 5;
        checkTeamSpirit(player.teamSpirit);
    }
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

    if(hit >= 55 && (choiceOpponentShot()))
    {
        opponent.score += 2;
        result += "Мяч в корзине! 2 очка!";
        opponent.teamSpiritOpponent += 5;
        checkTeamSpirit(opponent.teamSpiritOpponent);
        scoredOpponent = true;
        return result;
    }
    else if (hit > 65 && (!choiceOpponentShot()))
    {
        opponent.score += 3;
        result += "Мяч в корзине! 3 очка!";
        opponent.teamSpiritOpponent += 5;
        checkTeamSpirit(opponent.teamSpiritOpponent);
        scoredOpponent = true;
        return result;
    }
    else opponent.teamSpiritOpponent -= 5;
    result += "Это было близко, но мимо...";
    return result;
}

QString Basketball::autoStep()
{
    QString result;
    switch(currentStep)
    {
    case Step::PlayerRulesShot:
        result += showDefenseOpponent(opponent) + menu::rulesShot;
        currentStep = Step::PlayerInputShot;
        break;
    case Step::OpponentTurn:
    {
        switchDefenseOpponent(player, opponent);
        bool scoredOpponent = false;
        result = QString("%1 в атаке!\n").arg(opponent.name);
        result += opponentAttack(player, opponent, scoredOpponent);

        if (scoredOpponent)
        {
            result += QString("\nСчет на табло: %1 - %2").arg(player.score).arg(opponent.score);
        }
        else
        {
            if(rebound() && (stealed == false) && (blocked == false))
            {
                result += QString("Команда %1 подобрали мяч!").arg(opponent.name);
                currentStep = Step::OpponentTurn;
                return result;
            }
            else if ((stealed == false) && (blocked == false))
            {
                result += "Подбор за вами!\n";
            }
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

QString Basketball::showDefenseOpponent(const Opponent &opponent)
{
    QString showDefOpponent = "Защита противника: ";

    switch(opponent.defense)
    {
    case static_cast<int>(defense::PRESSING):
        showDefOpponent += defenseName.at(defense::PRESSING);
        break;
    case static_cast<int>(defense::PERSONAL_DEFENSE):
        showDefOpponent += defenseName.at(defense::PERSONAL_DEFENSE);
        break;
    case static_cast<int>(defense::ZONE_DEFENSE):
        showDefOpponent += defenseName.at(defense::ZONE_DEFENSE);
        break;
    case static_cast<int>(defense::NONE_DEFENSE):
        showDefOpponent += defenseName.at(defense::NONE_DEFENSE);
        break;
    default:
        break;
    }

    return showDefOpponent + "\n";
}

bool Basketball::rebound()
{
    engine::probalityRebound = distrib(gen);
    if (engine::probalityRebound > engine::PROCENT_REBOUND)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Basketball::choiceOpponentShot()
{
    int shot = distrib(gen);

    if(shot > 70) return true;
    return false;
}

void Basketball::checkTeamSpirit(int &teamSpirit)
{
    if(teamSpirit >= 20) teamSpirit = 20;
    else if(teamSpirit <= -10) teamSpirit = -10;
    else return;
}

QString Basketball::showDefensePlayer(const Player &player)
{
    QString showDefPlayer = "Ваша защита: ";


    switch(player.defense)
    {
    case static_cast<int>(defense::PRESSING):
        showDefPlayer += defenseName.at(defense::PRESSING);
        break;
    case static_cast<int>(defense::PERSONAL_DEFENSE):
        showDefPlayer += defenseName.at(defense::PERSONAL_DEFENSE);
        break;
    case static_cast<int>(defense::ZONE_DEFENSE):
        showDefPlayer += defenseName.at(defense::ZONE_DEFENSE);
        break;
    case static_cast<int>(defense::NONE_DEFENSE):
        showDefPlayer += defenseName.at(defense::NONE_DEFENSE);
        break;
    default:
        break;
    }

    return showDefPlayer + "\n";
}

