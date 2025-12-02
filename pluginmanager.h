#pragma once

#include <QObject>
#include <QPluginLoader>
#include <QHash>

#include "GameInterface.h"


class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QObject *parent = nullptr);

    bool loadAllPlugins();
    bool updatePlugins();
    bool loadPlugin(const QString& path);
    bool removePlugin(const QString& path);
    GameInterface* getInterface(const QString& path) const noexcept;

    PluginManager(const PluginManager& other) = delete;
    PluginManager& operator=(const PluginManager& other) = delete;

    PluginManager(const PluginManager&& other) = delete;
    PluginManager& operator=(const PluginManager&& other) = delete;

signals:
    // Закомментированные сигналы для расширения API после стабильной работы модуля расширить API
    // void pluginLoaded(const QString& name);
    void pluginsUpdate(const QVector<QString>& names);
    void pluginUnloaded(const QString& name);
    // void pluginRemoved(const QString& name);
    // void pluginError(const QString& msg);

private:
    bool unloadPlugin(const QString& path);

    struct PluginInfo
    {
        QPluginLoader* loader = nullptr; //после рефакторинга проследить зависимости, возможна замена на shared_ptr
        GameInterface* iface = nullptr;
        QString fullPath;

        PluginInfo(QPluginLoader* loader, GameInterface* iface, const QString& fullPath)
            : loader(loader), iface(iface), fullPath(fullPath) {}
        ~PluginInfo()
        {
            if(loader)
            {
                loader->unload();
                delete loader;
                loader = nullptr;
            }

        }

    };
    QHash<QString, PluginInfo*> loadedPlugins;
    QString folderPath = "plugins";
    QVector<QString> displayNames; //вектор для отдачи в GUI
};

