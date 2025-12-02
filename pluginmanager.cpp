#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>

#include "pluginmanager.h"
#include "LoggerMacros.h"


PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{}

bool PluginManager::loadAllPlugins()
{
    LOG_FUNC_START();
    // ---------- Очистка старого состояния ----------
    for(auto& pair : loadedPlugins)
    {
        if(pair->loader)
        {
            pair->loader->unload();
            delete pair->loader;
        }
    }
    loadedPlugins.clear();
    displayNames.clear();

    // Переходим в папку plugins (относительно applicationDirPath)
    QDir pluginsDir(QCoreApplication::applicationDirPath());
    if (!pluginsDir.cd(folderPath))
    {
        LOG_ERR(QString("Ошибка входа в директорию plugins!"));
        return false;
    }

    // Перечисляем файлы в каталоге (QDir::Files)
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    QString filter;
    for (const QString& fileName : entries)
    {
#ifdef Q_OS_WIN
        // Фильтруем по расширению DLL на Windows
        filter = ".dll";
#else
        filter = ".so";
#endif
        if(!fileName.endsWith(filter, Qt::CaseInsensitive))
            continue;
        const QString fullPath = pluginsDir.absoluteFilePath(fileName);

        // Создаём QPluginLoader без parent (мы сами будем управлять временем жизни loader)
        QPluginLoader* loader = new QPluginLoader(fullPath);

        // instance() попытается загрузить плагин и вернуть объект-инстанс.
        // Если плагин не загрузился — instance == nullptr.
        QObject* instance = loader->instance();

        if (!instance)
        {
            // Сообщаем в лог почему не удалось загрузить конкретный файл.
            LOG_ERR(QString("Не удалось загрузить плагин " + fullPath + "ошибка:" + loader->errorString()));
            delete loader;
            continue;
        }

        // Читаем MetaData для отображаемого имени плагина (опционально).
        QJsonObject meta = loader->metaData().value("MetaData").toObject();
        QString displayName = meta.value("name").toString();
        if(displayName.isEmpty()) { displayName = fileName; }

        // Пробуем привести instance к нашему интерфейсу GameInterface
        GameInterface* iface = qobject_cast<GameInterface*>(instance);
        if (!iface)
        {
            // Плагин не реализует требуемый интерфейс — выгружаем и освобождаем loader.
            LOG_ERR(QString("Плагин не реализует требуемый интерфейс: " + fullPath));
            loader->unload(); // пытаемся корректно выгрузить
            delete loader;
            continue;
        }

        // Всё успешно — сохраняем информацию о плагине.
        loadedPlugins.emplace(displayName, PluginInfo(loader, iface, fullPath));
        displayNames.push_back(displayName);
        LOG_INFO(QString("Плагин " + displayName + " успешно загружен!"));
    }

    emit pluginsUpdate(displayNames);
    return !loadedPlugins.empty();
}

bool PluginManager::updatePlugins()
{
    //TODO реализовать корректное удаление плагинов из системы, либо сделать буфер для сверки обновлений
    LOG_FUNC_START();
    // ---------- 0) Сохраняем текущий размер списка плагинов ----------
    // Нужен для того, чтобы после загрузки новых плагинов посчитать, сколько добавилось
    int currentPlugSize = loadedPlugins.size();

    for(auto iter = loadedPlugins.begin(); iter != loadedPlugins.end(); ++iter)
    {
        auto name = iter.key();
        unloadPlugin(name);
    }

    // ---------- 1) Пытаемся загрузить плагины ----------
    // Если loadAllPlugins() вернет false, значит загрузка не удалась
    if(!loadAllPlugins())
    {
        // Информируем пользователя о проблеме
        QMessageBox::information(nullptr, tr("Ошибка!"), tr("Не удалось загрузить плагин!"));
        LOG_ERR(QString("Не удалось загрузить плагин!"));
        return false; // Выходим из слота, дальше делать нечего
    }

    // ---------- 2) Получаем новый размер списка после апдейта ----------
    int updatePlugSize = loadedPlugins.size();

    // ---------- 3) Вычисляем, сколько новых плагинов добавлено ----------
    int resultUpdate = updatePlugSize - currentPlugSize;

    // ---------- 4) Формируем сообщение для пользователя ----------
    // Например: "Добавлено 3 игр!"
    QString result = "Добавлено " + QString::number(resultUpdate) + " игр!";

    //----------- 5) Обновляем вектор с именами плагинов, для отображения их в менеджере ----------
    emit pluginsUpdate(displayNames);

    // ---------- 6) Показываем информационное окно ----------
    // tr(result) не нужен, так как result уже QString, достаточно передать его напрямую
    QMessageBox::information(nullptr, tr("Информация!"), result);
    LOG_FUNC_END(result);
}

bool PluginManager::loadPlugin(const QString &path)
{
    if(loadedPlugins.contains(path))
    {
        LOG_WARN(QString("Плагин уже загружен: " + path));
        return false;
    }
    QPluginLoader* plugin = new QPluginLoader(loadedPlugins[path]->fullPath);
    QObject* instance = plugin->instance();

    if(!instance)
    {
        // Сообщаем в лог почему не удалось загрузить конкретный файл.
        LOG_ERR(QString("Не удалось загрузить плагин " + path + "ошибка:" + plugin->errorString()));
        delete plugin;
        return false;
    }

    QJsonObject json = plugin->metaData().value("MetaData").toObject();
    QString dispName = json.value("name").toString();
    if(dispName.isEmpty()) { dispName = path; }

    GameInterface* iface = qobject_cast<GameInterface*>(instance);
    if(!iface)
    {
        // Плагин не реализует требуемый интерфейс — выгружаем и освобождаем loader.
        LOG_ERR(QString("Плагин не реализует требуемый интерфейс: " + path));
        plugin->unload();
        delete plugin;
        return false;
    }

    loadedPlugins.emplace(dispName, PluginInfo(plugin, iface, path));
    LOG_INFO(QString("Плагин " + dispName + " успешно загружен!"));
    return true;
}

bool PluginManager::unloadPlugin(const QString &name)
{
    auto iter = loadedPlugins.find(name);
    if(iter == loadedPlugins.end())
    {
        LOG_WARN(QString("Плагин: %1 не найден").arg(name));
        return false;
    }

    PluginInfo* buff = iter.value();
    loadedPlugins.erase(iter);

    QObject* obj = buff->loader->instance();
    if(!obj)
        return false;

    obj->disconnect();

    delete buff;
    buff = nullptr;

    emit pluginUnloaded(name);

    return true;
}

bool PluginManager::removePlugin(const QString &path)
{
    LOG_FUNC_START();
    // ---------- 0) Проверка наличия плагина ----------
    if(!loadedPlugins.contains(path))
    {
        LOG_WARN(QString("Плагина нет в списке инициализированных!"));
        return false; // защита от некорректного вызова
    }

    auto iter = loadedPlugins.find(path);

    QString plugName = iter.key();

    // Получаем QPluginLoader*
    QPluginLoader* loader = iter.value()->loader;
    if (!loader)
    {
        QMessageBox::warning(nullptr, tr("Ошибка!"), tr("Загрузчик плагина отсутствует!"));
        LOG_ERR(QString("Загрузчик плагина отсутствует!"));
        return false;
    }

    // Получаем путь (строка) к файлу плагина — для логов и удаления.
    const QString fullP = iter.value()->fullPath;

    // ---------- 1) Аккуратно «отцепляемся» от instance ----------
    // Если instance() != nullptr, значит сейчас в памяти есть объект плагина.
    // Наша цель: гарантировать, что никто больше не держит ссылки/сигналы на объекты плагина,
    // потому что unload() удалит/освободит модуль, и обращение к таким объектам приведёт к крашу.
    QObject* instance = loader->instance(); // может быть nullptr, если уже выгружен

    if (instance)
    {
        // Отключаем все соединения связанных сигналов/слотов у instance
        // Это эквивалент instance->disconnect(nullptr, nullptr, nullptr, nullptr)
        // и защищает от случаев, когда слоты в основном приложении ещё подписаны на сигналы плагина.
        instance->disconnect();

        // Если мы в UI держали указатель на интерфейс (gameInterface), обнуляем его,
        // чтобы в дальнейшем не обращаться к освобождённому объекту.
        if (iter.value()->iface)
        {
            iter.value()->iface = nullptr;
        }
    }

    // ---------- 2) Выгружаем .dll через QPluginLoader ----------
    // QPluginLoader::unload() синхронно попытается выгрузить модуль и уничтожить instance.
    // Если какие-то объекты плагина живы (включая дочерние объекты, таймеры, запущенные потоки,
    // открытые файловые дескрипторы и т.п.), unload вернёт false.
    if (!loader->unload())
    {
        LOG_ERR(QString("[Plugin] ошибка при выгрузке " + path
                        + " ошибка:" + loader->errorString()));

        // Сообщаем пользователю — вот типичная причина, почему файл нельзя удалить:
        // остались живые объекты, таймеры или другой QPluginLoader держит библиотеку открытой.
        QMessageBox::warning(nullptr, tr("Ошибка!"),
                             tr("Не удалось выгрузить плагин (остались живые объекты)."));
        LOG_ERR(QString("Не удалось выгрузить плагин (остались живые объекты)."));
        return false;
    }

    // Windows держит дескриптор плагина до завершения процесса, пока нет отдельного сервера делаем rename
    const QString tombstone = path + ".deleted." + QString::number(QDateTime::currentMSecsSinceEpoch());
    if (QFile::rename(path, tombstone))
    {
        // rename успешен — считайте, что физически "удалили" плагин (переименовали).
        LOG_INFO(QString("[Plugin] файл переименован в " + tombstone));
    }
    else
    {
        // Если ни удалить, ни переименовать нельзя — сообщаем пользователю о причинах.
        QMessageBox::warning(nullptr, tr("Ошибка!"),
                             tr("Не удалось удалить/переименовать файл плагина.\n"
                                "Возможные причины: антивирус/индексатор блокирует файл, другой процесс держит DLL."));
        LOG_ERR(QString("Не удалось удалить/переименовать файл плагина.\n"
                        "Возможные причины: антивирус/индексатор блокирует файл, другой процесс держит DLL."));

        // Пытаемся восстановить состояние: reload() чтобы снова иметь рабочий instance.
        // TODO
        // В случае неудачи с удалением плагина, возможно не стоит его реалодить.
        // Это может создать ситуации, когда плагин не может быть удален, но после восстановления его невозможно удалить снова.
        // Возможно, это стоит пересмотреть или уточнить логику.
        if (!loader->load())
        {
            LOG_ERR(QString("[Plugin] не удалось восстановить состояние " + path + " ошибка:" + loader->errorString()));
            return false;
        }
        else
        {
            QObject* inst2 = loader->instance();
            GameInterface* iface2 = inst2 ? qobject_cast<GameInterface*>(inst2) : nullptr;
            if(iface2)
            {
                iter.value()->iface = iface2; // восстанавливаем указатель, если удалось загрузить
                LOG_FUNC_END(QString("Плагин " + plugName + " успешно востановлен!"));
                return false;
            }
        }
    }

    // ---------- 4) Всё успешно — чистим контейнеры/интерфейсы ----------
    // QPluginLoader уже выгрузил библиотеку (unload), мы можем удалить объект загрузчика.
    delete loader;

    // Удаляем запись о плагине из хеш мапы

    PluginInfo* buff = iter.value();
    loadedPlugins.erase(iter);

    // Очищаем память, чтобы не было утечек
    delete buff;

    //Меняем вектор имен, чтобы был актуальным
    displayNames.erase(std::remove_if(displayNames.begin(), displayNames.end(), [plugName](const QString& name) { return plugName == name; }), displayNames.end());

    //Отправляем сигнал в Manager чтобы удалить имя файла из GUI списка
    emit pluginsUpdate(displayNames);

    QMessageBox::information(nullptr, tr("Успех!"), tr("Плагин удалён"));
    LOG_FUNC_END(QString("Плагин " + plugName + " успешно удалён!"));
}

GameInterface *PluginManager::getInterface(const QString &path) const noexcept
{
    return loadedPlugins[path]->iface;
}
