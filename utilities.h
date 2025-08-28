#pragma once

#include <QVector>
#include <QListWidget>

#include "LoggerMacros.h"

class Utilities
{
public:
    Utilities() = default;
    ~Utilities() = default;

    // ============================================================================
    // Утилита: DeleteAll - освобождение памяти для вектора указателей
    // ----------------------------------------------------------------------------
    // Удаляет каждый указатель в vec и очищает вектор. Использовать аккуратно: этот код
    // предполагает, что мы владеем этими объектами и можем их delete.
    // ============================================================================
    template<typename T>
    static void DeleteAll(QVector<T*>& vec)
    {
        LOG_FUNC_START();
        int count = 0;
        for(auto& el : vec)
        {
            if(el) { delete el; } // безопасное удаление
            el = nullptr;
            ++count;
        }
        vec.clear();
        LOG_FUNC_END(QString("Удалено " + QString::number(count) + " элементов!"));
    }
    static void clearTombstone(const QString& dirName); // Утилита для удаления мусорных файлов в папке plugins
    static void clearItems(QListWidget* wid, QListWidget* wid2, QVector<QString> names);
};

