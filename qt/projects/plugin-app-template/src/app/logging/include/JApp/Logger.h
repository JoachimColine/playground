#pragma once

#include <QObject>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QLoggingCategory>
#include <QTimer>
#include <memory>

namespace JApp {

class Logger : public QObject
{
    Q_OBJECT

public:
    enum class LogLevel {
        Debug    = 0,
        Info     = 1,
        Warning  = 2,
        Critical = 3,
        Fatal    = 4
    };

    enum class OutputTarget {
        Console = 0x01,
        File    = 0x02,
        Both    = Console | File
    };

    struct LogConfig {
        LogConfig(){}
        LogLevel minLevel     = LogLevel::Debug;
        OutputTarget target   = OutputTarget::Both;
        QString logDirectory;
        QString logFilePrefix = "app";
        qint64 maxFileSize    = 10 * 1024 * 1024; // 10MB
        int maxFileCount      = 5;
        bool enableTimestamp  = true;
        bool enableCategory   = true;
        bool enableFunction   = true;
        bool enableLineNumber = true;
        bool enableThreadId   = true;
        bool autoFlush        = true;
        int flushIntervalMs   = 1000;
    };

    static Logger& instance();
    
    void initialize(const LogConfig& config = LogConfig());
    void shutdown();
    
    void setLogLevel(LogLevel level);
    void setOutputTarget(OutputTarget target);
    void setLogDirectory(const QString& directory);

private slots:
    void flushLogs();

private:
    explicit Logger(QObject* parent = nullptr);
    ~Logger();
    
    void setupMessageHandler();
    void rotateLogFile();
    QString formatLogMessage(LogLevel level, const QLoggingCategory& category, const QString& message, const QString& function, int line, const QString& file);
    void log(LogLevel level, const QLoggingCategory& category, const QString& message, const QString& function = QString(), int line = -1, const QString& file = QString());
    QString levelToString(LogLevel level);
    QString getCurrentLogFileName();
    void ensureLogDirectory();

    static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message);
    static LogLevel qtMsgTypeToLogLevel(QtMsgType type);

private:
    bool m_initialized = false;
    LogConfig m_config;
    std::unique_ptr<QFile> m_logFile;
    std::unique_ptr<QTextStream> m_logStream;
    QMutex m_mutex;
    QTimer* m_flushTimer;
    static Logger* s_instance;
};

inline bool hasFlag(Logger::OutputTarget target, Logger::OutputTarget flag) {
    return (static_cast<int>(target) & static_cast<int>(flag)) != 0;
}

}
