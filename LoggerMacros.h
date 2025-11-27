#pragma once

#include <QString>
#include "logger.h"

// Макросы логирования
#define LOG_FUNC_START() \
do { Logger::instance().logSystem(LogLevel::Info, QString("Начало метода %1").arg(Q_FUNC_INFO)); } while(0)

#define LOG_FUNC_END(msg) \
do { Logger::instance().logSystem(LogLevel::Info, QString("Метод %1 завершен успешно: %2").arg(Q_FUNC_INFO).arg(msg)); } while(0)

#define LOG_INFO(msg) \
do { Logger::instance().logSystem(LogLevel::Info, QString("%1: %2").arg(Q_FUNC_INFO).arg(msg)); } while(0)

#define LOG_WARN(msg) \
do { Logger::instance().logSystem(LogLevel::Warning, QString("%1: %2").arg(Q_FUNC_INFO).arg(msg)); } while(0)

#define LOG_ERR(msg) \
do { Logger::instance().logSystem(LogLevel::Error, QString("%1: %2").arg(Q_FUNC_INFO).arg(msg)); } while(0)

//Макросы логирования игрового процесса
#define START_GAME_LOG(gameName) \
do { Logger::instance().initGameLog(gameName); } while(0)

#define LOG_PC_ANSWER(msg, gameName) \
do { Logger::instance().log(QString("%1: %2").arg("Ответ игры").arg(msg), gameName); } while(0)

#define LOG_PLAYER_CHOICE(msg, gameName) \
do { Logger::instance().log(QString("%1: %2").arg("Ввод игрока").arg(msg), gameName); } while(0)

#define LOG_END_GAME(gameName) \
do { Logger::instance().log(QString("%1: ------------Игра завершена!------------").arg(gameName), gameName); } while(0)
