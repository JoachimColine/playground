#pragma once

#include <QObject>
#include <QList>
#include <QDir>
#include <plugins/interfaces/IPlugin.h>

class QPluginLoader;

class PluginManager : public QObject
{
    Q_OBJECT
	
	// TODO: plugins dir as q_property; signal for loading progress.

public:
    explicit PluginManager(QObject *parent = nullptr);
    ~PluginManager();

    bool loadAllPlugins();
    void unloadAllPlugins();
    QList<IPlugin*> loadedPlugins() const;

private:
    QList<QPluginLoader*> m_loaders;
    QList<IPlugin*> m_plugins;
};