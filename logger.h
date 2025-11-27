#pragma once

#include <QObject>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QDateTime>
#include <QDir>
#include <unordered_map>
//TODO переписать адекватно для логирования игр
// -----------------------------------------------------------------------------
// Уровни логирования
// -----------------------------------------------------------------------------
enum class LogLevel
{
    Info,
    Warning,
    Error
};

// -----------------------------------------------------------------------------
// Каналы логирования
// -----------------------------------------------------------------------------
enum class LogChannel
{
    System,
    Gameplay
};

// -----------------------------------------------------------------------------
// Logger — потокобезопасный синглтон с поддержкой
// * logs/system_log/system.log
// * logs/games_log/<GameName>/gameplay.log
// -----------------------------------------------------------------------------
class Logger : public QObject
{
    Q_OBJECT

public:
    static Logger& instance()
    {
        static Logger inst;
        return inst;
    }

    void setMinLevel(LogLevel lvl)
    {
        minLevel = lvl;
    }

    // Обязательный вызов перед началом логирования игры
    // Создает директорию игры и инициализирует стрим
    bool initGameLog(const QString& gameName)
    {
        QMutexLocker lock(&mtx);

        const QString gameDir = baseGamesDir + "/" + gameName;
        QDir().mkpath(gameDir);

        const QString logPath = gameDir + "/gameplay.log";

        auto file = std::make_unique<QFile>(logPath);
        if(!file->open(QIODevice::Append | QIODevice::Text))
            return false;

        auto stream = std::make_unique<QTextStream>(file.get());

        GameInfo gInfo;
        gInfo.gameFile = std::move(file);
        gInfo.gameStream = std::move(stream);
        gInfo.path = logPath;

        gameLogMap.emplace(gameName, std::move(gInfo));
        auto iter = gameLogMap.find(gameName);
        if(iter == gameLogMap.end())
            return false;
        *iter->second.gameStream << "\n-------------------Новая игра-------------------\n\n";
        iter->second.gameStream->flush();

        return true;
    }

    std::optional<QString> getPath(const QString& gameName) const noexcept
    {
        auto iter = gameLogMap.find(gameName);
        if(iter == gameLogMap.end())
            return std::nullopt;

        return iter->second.path;
    }

    // Публичный метод логирования
    void log(const QString& msg, const QString& gameName)
    {
        if(!ready) return;
        emit logSignal(LogChannel::Gameplay, {}, msg, gameName);
    }

    void logSystem(LogLevel lvl, const QString& msg)
    {
        if(!ready) return;
        emit logSignal(LogChannel::System, lvl, msg, {});
    }

signals:
    void logSignal(LogChannel channel, LogLevel lvl, const QString& msg, const QString& gameName);

private slots:
    void handleLog(LogChannel channel, LogLevel lvl, const QString& msg, const QString& gameName)
    {
        if(lvl < minLevel)
            return;

        QString prefix;
        switch (lvl)
        {
        case LogLevel::Info: prefix = "[INFO]"; break;
        case LogLevel::Warning: prefix = "[WARNING]"; break;
        case LogLevel::Error: prefix = "[ERROR]"; break;
        }

        const QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");


        QMutexLocker lock(&mtx);

        if(channel == LogChannel::System)
        {
            const QString full = time + " " + prefix + " " + msg + "\n";
            systemStream << full;
            systemStream.flush();
        }
        else
        {
            const QString full = msg + '\n';
            auto iter = gameLogMap.find(gameName);
            if(iter == gameLogMap.end())
                return;

            *iter->second.gameStream << full;
            iter->second.gameStream->flush();
        }
    }

private:
    Logger(QObject* parent = nullptr)
        : QObject(parent)
    {
        // --- Создаём директории ---
        QDir().mkpath("logs/system_log");
        QDir().mkpath("logs/games_log");

        baseSystemDir = "logs/system_log";
        baseGamesDir = "logs/games_log";

        // --- Подготавливаем system.log ---
        const QString sysLogPath = baseSystemDir + "/system.log";
        systemFile.setFileName(sysLogPath);

        if(!systemFile.open(QIODevice::Append | QIODevice::Text))
            return;

        systemStream.setDevice(&systemFile);

        // --- Стартуем поток ---
        workerThread = new QThread();
        this->moveToThread(workerThread);

        connect(
            this, &Logger::logSignal,
            this, &Logger::handleLog,
            Qt::QueuedConnection
            );

        workerThread->start();
        ready = true;

        logSystem(LogLevel::Info, "Logger initialized");
    }

    ~Logger()
    {
        if(workerThread)
        {
            workerThread->quit();
            workerThread->wait();
            delete workerThread;
        }
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    //структура логирования для игр
    struct GameInfo
    {
        std::unique_ptr<QFile> gameFile;
        std::unique_ptr<QTextStream> gameStream;
        QString path;
    };
    // system log
    QString baseSystemDir;
    QFile systemFile;
    QTextStream systemStream;

    // gameplay logs
    QString baseGamesDir;
    std::unordered_map<QString, GameInfo> gameLogMap;

    QMutex mtx;
    LogLevel minLevel = LogLevel::Info;
    QThread* workerThread = nullptr;
    bool ready = false;
};
