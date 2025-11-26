#include "echo.h"
#include <QDir>
#include <QSaveFile>
#include <QJsonDocument>

QString PluginEcho::gameInput(const QString &playerChoice)
{
    checkQuit = playerChoice;
    if(playerChoice == "") return nothing;
    else return add + playerChoice;
}

QString PluginEcho::startMessage() const
{
    return welcome;
}

bool PluginEcho::isOver() const
{
    return quit.toLower().trimmed() == checkQuit.toLower().trimmed();
}

QByteArray PluginEcho::saveState(const QString& content)
{

    // -------------------------------
    // Формируем JSON объект для сохранения данных
    // -------------------------------

    QJsonObject obj;
    obj["ui-content"] = content;                  // Текст на экране, на момент сохранения

    // -------------------------------
    // Превращаем JSON объект в QJsonDocument
    // -------------------------------
    QJsonDocument doc(obj);
    QByteArray gameData = doc.toJson();                // Преобразуем документ в последовательность байтов для записи в файл

    return gameData;                                  // Состояние успешно сохранено
}
std::optional<QString> PluginEcho::loadState(const QByteArray& gameData)
{

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(gameData, &err);

    if(err.error != QJsonParseError::NoError)
    {
        return std::nullopt;
    }

    if(!doc.isObject())
    {
        return std::nullopt;
    }

    QJsonObject obj = doc.object();

    if(obj.contains("ui-content") && obj["ui-content"].isString())
    {
        return obj["ui-content"].toString();
    }

    return std::nullopt;
}

int PluginEcho::getUndoAttempts() const noexcept
{
    return undoAttempts;
}
