#pragma once

#include <QLoggingCategory>
#include <QDebug>

#define FILE_CATEGORY_NAME() \
    []() { \
        static QByteArray nameBytes; \
        if (nameBytes.isEmpty()) { \
            QString file = __FILE__; \
            QString name = file.section('/', -1).section('.', 0, 0); \
            name = QString("app.%1").arg(name.toLower()); \
            nameBytes = name.toLatin1(); \
        } \
        return nameBytes.constData(); \
    }()

#define CURRENT_LOG_CATEGORY() \
    []() -> const QLoggingCategory& { \
        static QLoggingCategory category(FILE_CATEGORY_NAME()); \
        return category; \
    }()

#define LOG_DEBUG()    qCDebug(CURRENT_LOG_CATEGORY())
#define LOG_INFO()     qCInfo(CURRENT_LOG_CATEGORY())
#define LOG_WARN()     qCWarning(CURRENT_LOG_CATEGORY())
#define LOG_CRITICAL() qCCritical(CURRENT_LOG_CATEGORY())
