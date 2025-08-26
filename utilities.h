#pragma once
#include <QVector>
#include <QListWidget>

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
    template<typename T> // На будущее - переделать шаблон под все типы(QList, QSet и тд)
    static void DeleteAll(QVector<T*>& vec)
    {
        for(auto& el : vec)
        {
            if(el) { delete el; } // безопасное удаление
            el = nullptr;
        }
        vec.clear();
    }
    static void clearTombstone(const QString& dirName); // Утилита для удаления мусорных файлов в папке plugins
    static void clearItems(QListWidget* wid, QListWidget* wid2, const QVector<QString>& names); // Утилита удаляет элементы из двух QListWidget синхронно по списку имён.
};

