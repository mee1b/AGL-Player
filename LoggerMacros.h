#pragma once

#include <QString>
#include "logger.h"

// Макросы логирования
#define LOG_FUNC_START() \
do { Logger::getInstance().logMessage(logLevel::Info, QString("Начало метода %1").arg(Q_FUNC_INFO)); } while(0)

#define LOG_FUNC_END(msg) \
do { Logger::getInstance().logMessage(logLevel::Info, QString("Метод %1 завершен успешно: %2").arg(Q_FUNC_INFO).arg(msg)); } while(0)

#define LOG_INFO(msg) \
do { Logger::getInstance().logMessage(logLevel::Info, QString("%1: %2").arg(Q_FUNC_INFO).arg(msg)); } while(0)

#define LOG_WARN(msg) \
do { Logger::getInstance().logMessage(logLevel::Warning, QString("%1: %2").arg(Q_FUNC_INFO).arg(msg)); } while(0)

#define LOG_ERR(msg) \
do { Logger::getInstance().logMessage(logLevel::Error, QString("%1: %2").arg(Q_FUNC_INFO).arg(msg)); } while(0)

