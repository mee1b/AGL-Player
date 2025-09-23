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

bool PluginEcho::saveState(const QString &displayName, const QString& content)
{

    // -------------------------------
    // Формируем путь для сохранения
    // -------------------------------
    QString saveDir = "../save/" + displayName + "/";
    QString path = saveDir + "save.json";          // Полный путь до файла сохранения

    // -------------------------------
    // Проверяем, существует ли папка, если нет — создаём
    // -------------------------------

    QDir dir;                    // Класс для работы с директориями
    if(!dir.exists(saveDir))     // Проверка существования директории
    {
        dir.mkpath(saveDir);    // Создание всех промежуточных директорий (mkpath делает это рекурсивно)
    }

    // -------------------------------
    // Создаём объект QSaveFile для безопасной атомарной записи
    // -------------------------------
    QSaveFile file(path);             // QSaveFile создаёт временный файл в той же папке
    if(!file.open(QIODevice::WriteOnly)) // Открываем файл только для записи
    {
        return false;                 // Если открыть файл не удалось — выходим с ошибкой
    }

    // -------------------------------
    // Формируем JSON объект для сохранения данных
    // -------------------------------

    QJsonObject obj;
    obj["ui-content"] = content;                  // Текст на экране, на момент сохранения

    // -------------------------------
    // Превращаем JSON объект в QJsonDocument
    // -------------------------------
    QJsonDocument doc(obj);
    QByteArray arr = doc.toJson();                // Преобразуем документ в последовательность байтов для записи в файл

    // -------------------------------
    // Пишем байты в файл и проверяем, что запись прошла полностью
    // -------------------------------
    qint64 written = file.write(arr);             // Записываем данные в временный файл
    if(written != arr.size())                     // Если записано не всё
    {
        return false;                             // Возвращаем false, чтобы сигнализировать о проблеме
    }

    // -------------------------------
    // Завершаем запись — временный файл заменяет основной
    // -------------------------------
    file.commit();                                // Атомарная замена старого файла новым

    return true;                                  // Состояние успешно сохранено
}
std::optional<QString> PluginEcho::loadState(const QString &displayName)
{
    QString saveDir = "../save/" + displayName + "/";
    QString path = saveDir + "save.json";

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        return std::nullopt;
    }

    QByteArray arr = file.readAll();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(arr, &err);

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
