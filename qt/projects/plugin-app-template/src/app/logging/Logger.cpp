#include "Logger.h"
#include <QApplication>
#include <QThread>
#include <QFileInfo>
#include <QDebug>
#include <iostream>

using namespace JApp;

Logger* Logger::s_instance = nullptr;

Logger::Logger(QObject* parent)
    : QObject(parent)
    , m_flushTimer(new QTimer(this))
{
    connect(m_flushTimer, &QTimer::timeout, this, &Logger::flushLogs);
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
            m_logFile = std::make_unique<QFile>(getCurrentLogFileName());
            if (m_logFile->open(QIODevice::WriteOnly | QIODevice::Append)) {
                m_logStream = std::make_unique<QTextStream>(m_logFile.get());
                m_logStream->setEncoding(QStringConverter::Utf8);
            } else {
                qWarning() << "Failed to open log file:" << m_logFile->fileName();
            }
        }

        // Setup Qt message handler
        setupMessageHandler();

        // Start flush timer if auto-flush is enabled
        if (m_config.autoFlush && m_config.flushIntervalMs > 0) {
            m_flushTimer->start(m_config.flushIntervalMs);
        }
    } // release mutex.
    
    // Log initialization
    log(LogLevel::Info, QLoggingCategory("logger"), 
        QString("Logger initialized - Target: %1, Level: %2, Directory: %3")
        .arg(static_cast<int>(m_config.target))
        .arg(static_cast<int>(m_config.minLevel))
        .arg(m_config.logDirectory));
}

void Logger::shutdown()
{
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

void Logger::log(LogLevel level, const QLoggingCategory& category, 
                const QString& message, const QString& function, 
                int line, const QString& file)
{
    // Check if we should log this level
    if (level < m_config.minLevel) {
        return;
    }
    
    QMutexLocker locker(&m_mutex);
    
    QString formattedMessage = formatLogMessage(level, category, message, function, line, file);
    
    // Console output
    if (hasFlag(m_config.target, OutputTarget::Console)) {
        if (level >= LogLevel::Critical) {
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
        
        if (!m_config.autoFlush) {
            m_logStream->flush();
        }
    }
}

void Logger::flushLogs()
{
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
    
    LogLevel level = qtMsgTypeToLogLevel(type);
    QLoggingCategory category(context.category ? context.category : "qt");
    
    QString function = context.function ? QString(context.function) : QString();
    QString file = context.file ? QString(context.file) : QString();
    
    s_instance->log(level, category, message, function, context.line, file);
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
    if (!m_logFile || !m_logStream) {
        return;
    }
    
    m_logStream->flush();
    m_logFile->close();
    
    QString baseFileName = m_config.logFilePrefix;
    QString currentFile = getCurrentLogFileName();
    
    // Rotate existing files
    for (int i = m_config.maxFileCount - 1; i >= 1; --i) {
        QString oldFile = QString("%1/%2_%3.log").arg(m_config.logDirectory).arg(baseFileName).arg(i);
        QString newFile = QString("%1/%2_%3.log").arg(m_config.logDirectory).arg(baseFileName).arg(i + 1);
        
        if (QFile::exists(oldFile)) {
            QFile::remove(newFile); // Remove if exists
            QFile::rename(oldFile, newFile);
        }
    }
    
    // Move current file to .1
    QString rotatedFile = QString("%1/%2_1.log").arg(m_config.logDirectory).arg(baseFileName);
    QFile::remove(rotatedFile);
    QFile::rename(currentFile, rotatedFile);
    
    // Open new current file
    m_logFile = std::make_unique<QFile>(currentFile);
    if (m_logFile->open(QIODevice::WriteOnly | QIODevice::Append)) {
        m_logStream = std::make_unique<QTextStream>(m_logFile.get());
        m_logStream->setEncoding(QStringConverter::Utf8);
    }
}

QString Logger::formatLogMessage(LogLevel level, const QLoggingCategory& category,
                               const QString& message, const QString& function,
                               int line, const QString& file)
{
    QStringList parts;
    
    // Timestamp
    if (m_config.enableTimestamp) {
        parts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    }
    
    // Level
    parts << QString("[%1]").arg(levelToString(level));
    
    // Category
    if (m_config.enableCategory && category.categoryName() != nullptr) {
        parts << QString("[%1]").arg(category.categoryName());
    }
    
    // Thread ID
    if (m_config.enableThreadId) {
        parts << QString("[Thread:%1]").arg(reinterpret_cast<quintptr>(QThread::currentThread()), 0, 16);
    }
    
    // Function and line
    if (m_config.enableFunction && !function.isEmpty()) {
        QString funcInfo = function;
        if (m_config.enableLineNumber && line > 0) {
            funcInfo += QString(":%1").arg(line);
        }
        parts << QString("[%1]").arg(funcInfo);
    }
    
    // Message
    parts << message;
    
    return parts.join(" ");
}

QString Logger::levelToString(LogLevel level)
{
    switch (level) {
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warning:  return "WARN";
        case LogLevel::Critical: return "ERROR";
        case LogLevel::Fatal:    return "FATAL";
        default:                 return "UNKNOWN";
    }
}

QString Logger::getCurrentLogFileName()
{
    return QString("%1/%2.log").arg(m_config.logDirectory).arg(m_config.logFilePrefix);
}

void Logger::ensureLogDirectory()
{
    QDir dir;
    if (!dir.exists(m_config.logDirectory)) {
        dir.mkpath(m_config.logDirectory);
    }
}
