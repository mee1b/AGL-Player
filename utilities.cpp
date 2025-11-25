#include <QDir>

#include "utilities.h"


// ============================================================================
// Утилита: clearTombstone
// ----------------------------------------------------------------------------
// Автоматически очищает директорию плагинов от всех временных файлов,
// созданных при неудачном удалении плагинов (с именами вида *.deleted.*).
// ============================================================================
void Utilities::clearTombstone(const QString& dirName)
{
    LOG_FUNC_START();
    // Создаём объект QDir для папки с названием dirName
    QDir pluginsDir(dirName);

    // Получаем список всех файлов в этой папке, имена которых содержат "deleted"
    // QDir::Files — возвращаем только файлы, игнорируем папки
    QStringList tombstone = pluginsDir.entryList(QStringList() << "*deleted*", QDir::Files);

    // Перебираем каждый файл из списка
    for(auto& file : tombstone)
    {
        LOG_INFO(QString(file + " удален!"));
        // Удаляем файл с диска, получая его полный путь через pluginsDir.absoluteFilePath(file)
        QFile::remove(pluginsDir.absoluteFilePath(file));
    }
    LOG_FUNC_END(QString("'Мусорные' файлы прошлой сессии удалены!"));
}

//  Utilities::clearItems
//  Удаляет элементы из двух QListWidget по именам из вектора names.
//  *
//  Функция проходит по вектору names и сравнивает каждый элемент с последним
//  элементом QListWidget. Если совпадение найдено, элемент удаляется из обоих виджетов.
//  *
//  wid Первый QListWidget, из которого удаляются элементы.
//  wid2 Второй QListWidget, из которого удаляются элементы.
//  names Вектор имен элементов, которые нужно удалить.

void Utilities::clearItems(QListWidget* wid, QListWidget* wid2, QVector<QString> names)
{
    LOG_FUNC_START(); // Начало функции

    // Проверка валидности указателей
    if(wid == nullptr || wid2 == nullptr)
    {
        LOG_ERR(QString("Невалидные указатель/ли QListWidget!"));
        return;
    }

    // Индекс последнего элемента в векторе names
    int size = static_cast<int>(names.size()) - 1;

    // Проходим по всем именам
    for(const auto& name : names)
    {
        if(size < 0) { break; } // Если индекс вышел за пределы — прерываем цикл

        // Сравниваем имя с элементом QListWidget
        if(name == wid->item(size)->text())
        {
            LOG_INFO(QString(name + " удален!")); // Логируем удаление
            delete wid->takeItem(size);   // Удаляем из первого виджета
            delete wid2->takeItem(size);  // Удаляем из второго виджета
            --size;                   // Переходим к предыдущему элементу
        }
    }

    LOG_FUNC_END(QString("QListWidgets очищены!")); // Конец функции
}
