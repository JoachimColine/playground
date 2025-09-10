#include "JApp/Logger.h"
#include <QApplication>
#include <QThread>
#include <QFileInfo>
#include <QDebug>
#include <JApp/Log.h>
#include <iostream>

using namespace JApp;

Logger* Logger::s_instance = nullptr;

Logger::Logger(QObject* parent)
    : QObject(parent)
    , m_flushTimer(new QTimer(this))
    , m_initialized(false)
{
}

Logger::~Logger()
{
    shutdown();
}

Logger& Logger::instance()
{
    if (!s_instance) {
        s_instance = new Logger();
    }
    return *s_instance;
}

void Logger::initialize(const LogConfig& config)
{
    if (m_initialized) return;
    {
        QMutexLocker locker(&m_mutex);

        m_config = config;

        // Set default log directory if not specified
        if (m_config.logDirectory.isEmpty()) {
            m_config.logDirectory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs";
        }

        ensureLogDirectory();

        // Setup file logging if enabled
        if (hasFlag(m_config.target, OutputTarget::File)) {
            m_logFile = std::make_unique<QFile>(createLogFilePath());
            if (m_logFile->open(QIODevice::WriteOnly | QIODevice::Append)) {
                m_logStream = std::make_unique<QTextStream>(m_logFile.get());
                m_logStream->setEncoding(QStringConverter::Utf8);
            } else {
                std::cout << "Failed to open log file: " << m_logFile->fileName().toStdString() << std::endl;
            }
        }

        // Setup Qt message handler
        setupMessageHandler();

        m_flushTimer->setInterval(m_config.flushIntervalMs);
        connect(m_flushTimer, &QTimer::timeout, this, &Logger::flushLogs);
        m_flushTimer->start();

    } // Release mutex.

    m_initialized = true;

    LOG_INFO() << QString("Logger initialized - Target: %1, Level: %2, Directory: %3")
                  .arg(static_cast<int>(m_config.target))
                  .arg(static_cast<int>(m_config.minLevel))
                  .arg(m_config.logDirectory);
}

void Logger::shutdown()
{
    if (!m_initialized) return;

    QMutexLocker locker(&m_mutex);
    
    if (m_flushTimer->isActive()) {
        m_flushTimer->stop();
    }
    
    if (m_logStream) {
        m_logStream->flush();
        m_logStream.reset();
    }
    
    if (m_logFile) {
        m_logFile->close();
        m_logFile.reset();
    }
    
    // Restore default message handler
    qInstallMessageHandler(nullptr);
}

void Logger::setLogLevel(LogLevel level)
{
    QMutexLocker locker(&m_mutex);
    m_config.minLevel = level;
}

void Logger::setOutputTarget(OutputTarget target)
{
    QMutexLocker locker(&m_mutex);
    m_config.target = target;
}

void Logger::setLogDirectory(const QString& directory)
{
    QMutexLocker locker(&m_mutex);
    m_config.logDirectory = directory;
    ensureLogDirectory();
}

void Logger::handleLog(const Log& log)
{
    if (!m_initialized) return;

    // Check if we should log this level
    if (log.level < m_config.minLevel) {
        return;
    }
    
    QMutexLocker locker(&m_mutex);
    
    QString formattedMessage = formatLog(log);
    
    // Console output
    if (hasFlag(m_config.target, OutputTarget::Console)) {
        if (log.level >= LogLevel::Critical) {
            std::cerr << formattedMessage.toStdString() << std::endl;
        } else {
            std::cout << formattedMessage.toStdString() << std::endl;
        }
    }
    
    // File output
    if (hasFlag(m_config.target, OutputTarget::File) && m_logStream) {
        // Check file size and rotate if necessary
        if (m_logFile->size() > m_config.maxFileSize) {
            rotateLogFile();
        }
        
        *m_logStream << formattedMessage << Qt::endl;
    }
}

void Logger::flushLogs()
{
    if (!m_initialized) return;

    QMutexLocker locker(&m_mutex);
    if (m_logStream) {
        m_logStream->flush();
    }
}

void Logger::setupMessageHandler()
{
    qInstallMessageHandler(&Logger::messageHandler);
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    if (!s_instance) {
        return;
    }

    Log log {
        QDateTime::currentDateTime(),
        context.category ? QString(context.category) : QString("?"),
        qtMsgTypeToLogLevel(type),
        context.file ? QString(context.file) : QString("?"),
        context.function ? QString(context.function) : QString("?"),
        context.line,
        message
    };
    
    s_instance->handleLog(log);
}

Logger::LogLevel Logger::qtMsgTypeToLogLevel(QtMsgType type)
{
    switch (type) {
        case QtDebugMsg:    return LogLevel::Debug;
        case QtInfoMsg:     return LogLevel::Info;
        case QtWarningMsg:  return LogLevel::Warning;
        case QtCriticalMsg: return LogLevel::Critical;
        case QtFatalMsg:    return LogLevel::Fatal;
        default:            return LogLevel::Debug;
    }
}

void Logger::rotateLogFile()
{
    if (!m_logFile || !m_logStream) return;
    
    m_logStream->flush();
    m_logFile->close();
    m_logFile = std::make_unique<QFile>(createLogFilePath());
    if (m_logFile->open(QIODevice::WriteOnly | QIODevice::Append)) {
        m_logStream = std::make_unique<QTextStream>(m_logFile.get());
        m_logStream->setEncoding(QStringConverter::Utf8);
    }
}

QString Logger::formatLog(const Log& log)
{
    QStringList parts;
    
    // Timestamp
    if (m_config.enableTimestamp) {
        parts << log.timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz");
    }
    
    // Level
    parts << QString("%1").arg(levelToString(log.level));

    // Category
    parts << QString("%1").arg(log.category);
    
    // Function and line
    if (m_config.enableFunction && !log.function.isEmpty()) {
        QString funcInfo = log.function;
        if (m_config.enableLineNumber && log.line > 0) {
            funcInfo += QString(":%1").arg(log.line);
        }
        parts << QString("%1").arg(funcInfo);
    }
    
    // Message
    parts << log.message;

    // Thread ID
    if (m_config.enableThreadId) {
        parts << QString("%1").arg(reinterpret_cast<quintptr>(QThread::currentThread()), 0, 16);
    }
    
    return parts.join(" | ");
}

QString Logger::levelToString(LogLevel level)
{
    switch (level) {
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO ";
        case LogLevel::Warning:  return "WARN ";
        case LogLevel::Critical: return "ERROR";
        case LogLevel::Fatal:    return "FATAL";
        default:                 return "?    ";
    }
}

QString Logger::createLogFilePath()
{
    return QString("%1/%2_%3.log")
        .arg(m_config.logDirectory)
        .arg(m_config.logFilePrefix)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh'h'mm'm'ss's'"));
}

void Logger::ensureLogDirectory()
{
    QDir dir;
    if (!dir.exists(m_config.logDirectory)) {
        dir.mkpath(m_config.logDirectory);
    }
}
