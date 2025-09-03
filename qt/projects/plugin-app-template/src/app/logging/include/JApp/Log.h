#pragma once

#include "JApp/Internal/LogUtils.h"
#include <QLoggingCategory>
#include <QDebug>

#define CATEGORY_NAME_FROM_PATH() \
    []() { \
            static constexpr auto name = JApp::Internal::LogUtils::categoryNameFromPath(__FILE__); \
            static constexpr const char* str = name; \
            return str; \
    }()

#define CURRENT_LOG_CATEGORY() \
    []() -> const QLoggingCategory& { \
            static const QLoggingCategory category(CATEGORY_NAME_FROM_PATH()); \
            return category; \
    }()

#define LOG_DEBUG()    qCDebug(CURRENT_LOG_CATEGORY())
#define LOG_INFO()     qCInfo(CURRENT_LOG_CATEGORY())
#define LOG_WARN()     qCWarning(CURRENT_LOG_CATEGORY())
#define LOG_CRITICAL() qCCritical(CURRENT_LOG_CATEGORY())
