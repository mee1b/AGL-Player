#include <QSaveFile>
#include <QCoreApplication>
#include <QDir>
#include <QPlainTextEdit>
#include <QListWidgetItem>

#include "GameInterface.h"
#include "statemanager.h"
#include "LoggerMacros.h"

StateManager::StateManager(QObject *parent)
    : QObject{parent}
{}

bool StateManager::snapshot(GameInterface* iface, QListWidgetItem* item, QPlainTextEdit* header)
{
    LOG_FUNC_START();

    // -------------------------------
    // Пишем в буфер для проверки состояния игры
    // -------------------------------
    QByteArray buffSave = iface->saveState(header->toPlainText());
    if(buffSave.isEmpty())
    {
        LOG_ERR(QString("Не удалось сохранить состояние игры " + item->text()));
        return false;
    }

    undoStack.push(buffSave);

    // -------------------------------
    // Так как появляется новое undo состояние, чистим старый redo stack
    // -------------------------------
    while(!redoStack.empty())
    {
        redoStack.pop();
    }

    LOG_FUNC_END(QString("undoStack успешно обновлен!"));
    return true;
}

std::optional<QByteArray> StateManager::undo(GameInterface *iface, QListWidgetItem *item, QPlainTextEdit* header)
{
    LOG_FUNC_START();
    if(undoStack.empty())
    {
        LOG_WARN(QString("undo stack пуст"));
        return std::nullopt;
    }

    //получаем текущее состояние игры для сохранения в redo stack
    if(!iface)
        return std::nullopt;
    if(!item)
        return std::nullopt;
    if(!header)
        return std::nullopt;

    auto currentState = iface->saveState(header->toPlainText());

    //Если не пишем redo значит и не делаем undo
    if(currentState.isEmpty())
    {
        LOG_ERR(QString("Текущее состояние игры " + item->text() + " пустое"));
        return std::nullopt;
    }

    //сохраняем тукущее состояние в redo stack
    redoStack.push(currentState);

    auto buffState = undoStack.top();
    undoStack.pop();


    if(buffState.isEmpty())
    {
        LOG_ERR(QString("Ошибка undo операции с игрой " + item->text()));
        return std::nullopt;
    }

    LOG_FUNC_END(QString("undo операция успешна, игра " + item->text()));

    return buffState;
}

std::optional<QByteArray> StateManager::redo(GameInterface *iface, QListWidgetItem *item, QPlainTextEdit* header)
{
    if(redoStack.empty())
    {
        LOG_WARN(QString("redo stack пуст"));
        return std::nullopt;
    }

    if(!iface)
        return std::nullopt;
    if(!item)
        return std::nullopt;

    auto currentState = iface->saveState(header->toPlainText());
    if(currentState.isEmpty())
    {
        LOG_ERR(QString("Текущее состояние игры " + item->text() + " пустое"));
        return std::nullopt;
    }
    undoStack.push(currentState);

    auto bufState = redoStack.top();
    redoStack.pop();

    if(bufState.isEmpty())
    {
        LOG_ERR(QString("Ошибка redo операции с игрой " + item->text()));
        return std::nullopt;
    }

    LOG_FUNC_END(QString("redo операция успешна, игра " + item->text()));

    return bufState;
}

bool StateManager::save(GameInterface* iface, QListWidgetItem* item, QPlainTextEdit* header) const
{
    LOG_FUNC_START();
    // -------------------------------
    // Формируем путь для сохранения
    // -------------------------------
    QString baseDir = QCoreApplication::applicationDirPath();
    QString saveDir = baseDir + "/save/" + item->text() + "/"; // Папка для сохранений конкретного плагина
    QString path = saveDir + "gameSave.save";                  // Полный путь до файла сохранения

    // -------------------------------
    // Проверяем, существует ли папка, если нет — создаём
    // -------------------------------
    QDir dir;                // Класс для работы с директориями
    if(!dir.exists(saveDir)) // Проверка существования директории
    {
        dir.mkpath(saveDir); // Создание всех промежуточных директорий (mkpath делает это рекурсивно)
    }

    // -------------------------------
    // Создаём объект QSaveFile для безопасной атомарной записи
    // -------------------------------

    if(QFile::exists(path)) //Если файл уже существует - удаляем его
    {
        QFile::remove(path);
    }

    QSaveFile file(path);                // QSaveFile создаёт временный файл в той же папке
    if(!file.open(QIODevice::WriteOnly)) // Открываем файл только для записи
    {
        LOG_ERR(QString("Ошибка открытия файла по пути: " + path + " " + file.errorString()));
        return false;                    // Если открыть файл не удалось — логируем и выходим с ошибкой
    }

    // -------------------------------
    // Пишем байты в файл и проверяем, что запись прошла полностью
    // -------------------------------
    auto gameData = iface->saveState(header->toPlainText());           // Берем состояние текущее состояние игры

    qint64 wrt = file.write(gameData);
    if(wrt != gameData.size())
    {
        LOG_ERR(QString("Файл " + path + " потерял данные при сохранении!"));
        return false;                             // Если записано меньше, чем должно было логируем и выходим с ошибкой
    }

    // -------------------------------
    // Завершаем запись — временный файл заменяет основной
    // -------------------------------

    if(!file.commit())
    {
        LOG_ERR(QString("Не удалось сохранить файл по пути " + path));
        return false;
    }

    LOG_FUNC_END(QString("Файл успешно сохранен по пути: " + path));
    return true;
}

std::optional<QString> StateManager::load(GameInterface *iface, QListWidgetItem *item) const
{
    LOG_FUNC_START();
    // -------------------------------
    // Формируем путь для загрузки
    // -------------------------------
    QString baseDir = QCoreApplication::applicationDirPath();
    QString loadDir = baseDir + "/save/" + item->text() + "/";
    QString path = loadDir + "gameSave.save";

    // -------------------------------
    // Проверяем, существует ли папка, если нет — выходим с ошибкой
    // -------------------------------
    QFile file(path);
    if(!QFile::exists(path))
    {
        LOG_ERR(QString("Путь к файлу неправильный: " + path + " " + file.errorString()));
        return std::nullopt;
    }

    // -------------------------------
    // Открываем файл сохранения для чтения
    // -------------------------------
    if(!file.open(QIODevice::ReadOnly))
    {
        LOG_ERR(QString("Ошибка открытия файла по пути: " + path + " " + file.errorString()));
        return std::nullopt;
    }

    // -------------------------------
    // Читаем из файла данные для загрузки
    // -------------------------------
    QByteArray gameData = file.readAll();
    if(gameData.isEmpty())
    {
        LOG_WARN(QString("Файл по пути " + path + " не имеет данных"));
        return std::nullopt;
    }

    // -------------------------------
    // Загружаем и проверяем все ли успешно
    // -------------------------------
    auto check = iface->loadState(gameData);
    if(!check)
    {
        LOG_ERR(QString("Ошибка загрузки файла по пути " + path));
        return std::nullopt;
    }

    LOG_FUNC_END(QString("Файл успешно загружен по пути: " + path));
    return check.value();
}
