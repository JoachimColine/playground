#pragma once

#include <QLoggingCategory>
#include <QDebug>

constexpr const char* categoryNameFromPath(const char* path) {
    const char* file = path;
    for (const char* p = path; *p; ++p) {
        if (*p == '/' || *p == '\\') {
            file = p + 1;
        }
    }
    return file;
}

#define CURRENT_LOG_CATEGORY() \
    []() -> const QLoggingCategory& { \
        constexpr auto categoryName = categoryNameFromPath(__FILE__); \
        static const QLoggingCategory category(categoryName); \
        return category; \
    }()

#define LOG_DEBUG()    qCDebug(CURRENT_LOG_CATEGORY())
#define LOG_INFO()     qCInfo(CURRENT_LOG_CATEGORY())
#define LOG_WARN()     qCWarning(CURRENT_LOG_CATEGORY())
#define LOG_CRITICAL() qCCritical(CURRENT_LOG_CATEGORY())
