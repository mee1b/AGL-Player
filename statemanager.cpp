#include <QSaveFile>
#include <QCoreApplication>
#include <QDir>
#include <QPlainTextEdit>
#include <QListWidgetItem>
#include <QDataStream>
#include <QCryptographicHash>

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

    undoList.emplace_back(buffSave);

    // -------------------------------
    // Так как появляется новое undo состояние, чистим старый redo stack
    // -------------------------------
    while(!redoStack.empty())
    {
        redoStack.pop();
    }

    LOG_FUNC_END(QString("undoList успешно обновлен!"));
    return true;
}

std::optional<QByteArray> StateManager::undo(GameInterface *iface, QListWidgetItem *item, QPlainTextEdit* header)
{
    LOG_FUNC_START();
    if(undoList.empty())
    {
        LOG_WARN(QString("undo list пуст"));
        return std::nullopt;
    }

    while(undoList.size() > iface->getUndoAttempts())
    {
        undoList.pop_front();
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

    auto buffState = undoList.back();
    undoList.pop_back();


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
    undoList.emplace_back(currentState);

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

bool StateManager::save(GameInterface* iface, QListWidgetItem* item, QPlainTextEdit* header, quint32 version) const
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

    auto gameData = iface->saveState(header->toPlainText());  // Берем состояние текущее состояние игры

    //Вычисляем хэш от данных
    QByteArray hash = QCryptographicHash::hash(gameData, QCryptographicHash::Sha256);
    if(hash.size() != 32)
    {
        LOG_ERR(QString("Ошибка создания хэша для игры " + item->text() ));
        return false;
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

    //Создаем QDataStream для бинарной записи
    QDataStream out(&file);

    //Устанавливаем версию для совместимости
    out.setVersion(QDataStream::Qt_6_8);

    //Делам префикс данных
    out.writeRawData("AGLS", 4);

    //пишем версию метода сохранения(нужна для будующей совместимости) в поток
    out << version;

    //записываем хэш файла длина 32 байта - фиксирована
    out.writeRawData(hash.constData(), static_cast<quint32>(hash.size()));

    //записываем размер gameData
    out << static_cast<quint32>(gameData.size());

    //записываем gameData в поток
    out.writeRawData(gameData.constData(), static_cast<quint32>(gameData.size()));

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
    // Проверяем, существует ли файл, если нет — выходим с ошибкой
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

    //создаем поток на чтение
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_8);

    //Читаем префикс
    char prefix[4];
    if(in.readRawData(prefix, 4) != 4)
    {
        LOG_ERR(QString("Нарушение целостности префикса игры " + item->text()));
        return std::nullopt;
    }
    if(std::memcmp(prefix, "AGLS", 4) != 0) //сравниваем буферы префикса, чтобы убедится что это наш файл
    {
        LOG_ERR(QString("Неверный префикс игры " + item->text()));
        return std::nullopt;
    }

    //Читаем версию. При изменении версии добавится switch/case для совместимости
    quint32 version;
    in >> version;

    //Читаем хэш из файла
    QByteArray fileHash(32, 0);
    if(in.readRawData(fileHash.data(), 32) != 32)
    {
        LOG_ERR(QString("Ошибка целостности хэша игры " + item->text()));
        return std::nullopt;
    }

    //Читаем размер данных
    quint32 gameDataSize;
    in >> gameDataSize;

    // Читаем из файла данные для загрузки
    QByteArray gameData;
    gameData.resize(gameDataSize);
    if(in.readRawData(gameData.data(), gameDataSize) != static_cast<int>(gameDataSize))
    {
        LOG_ERR(QString("Ошибка чтения. Данные не равны фактическому размер. Игра " + item->text()));
        return std::nullopt;
    }

    //Сравниваем хэши из файла и фактических хэш данных
    QByteArray currentHash = QCryptographicHash::hash(gameData, QCryptographicHash::Sha256);
    if(currentHash != fileHash)
    {
        LOG_ERR(QString("Хэш поврежден. Игра " + item->text()));
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
