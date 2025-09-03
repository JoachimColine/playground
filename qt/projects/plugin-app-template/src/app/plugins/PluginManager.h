#pragma once

#include <QObject>
#include <QList>
#include <QDir>

class QPluginLoader;

namespace JApp {

class IPlugin;

class PluginManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString directory READ directory CONSTANT)
    Q_PROPERTY(qreal loadingProgress READ loadingProgress NOTIFY loadingProgressChanged)
    Q_PROPERTY(qreal unloadingProgress READ unloadingProgress NOTIFY unloadingProgressChanged)

public:
    explicit PluginManager(QString directory, QObject *parent = nullptr);
    ~PluginManager();

    QString directory() const;

    qreal loadingProgress() const;
    qreal unloadingProgress() const;

    bool loadAllPlugins();
    void unloadAllPlugins();
    QList<JApp::IPlugin*> loadedPlugins() const;

signals:
    void loadingProgressChanged(qreal progress);
    void unloadingProgressChanged(qreal progress);

private:
    void setLoadingProgress(qreal progress);
    void setUnloadingProgress(qreal progress);

private:
    QString m_directory;
    QList<QPluginLoader*> m_loaders;
    QList<JApp::IPlugin*> m_plugins;
};

}
