#pragma once

#include <QLoggingCategory>
#include <QDebug>

/* Macros for logging.
 * Logging categories are generated automatically and at compile time.
 */

namespace JApp {
    namespace Internal {
        constexpr size_t findLastSeparator(const char* path) {
            size_t i = 0;
            size_t lastSep = 0;

            while (path[i]) {
                if (path[i] == '/' || path[i] == '\\') {
                    lastSep = i;
                }
                ++i;
            }

            return lastSep;
        }

        constexpr size_t findPenultimateSeparator(const char* path, size_t lastSep) {
            size_t penultimateSep = 0;

            for (size_t i = 0; i < lastSep; ++i) {
                if (path[i] == '/' || path[i] == '\\') {
                    penultimateSep = i;
                }
            }

            return penultimateSep;
        }

        template <size_t N>
        struct StringLiteral {
            char data[N];
            constexpr StringLiteral() : data{} {}
            constexpr operator const char*() const {
                return data;
            }
        };

        template <size_t MaxSize = 64>
        constexpr auto categoryNameFromPath(const char* path) {
            StringLiteral<MaxSize> result = {};

            const size_t lastSep = findLastSeparator(path);
            if (lastSep == 0) {
                result.data[0] = '.';
                result.data[1] = '\0';
                return result;
            }

            const size_t penultimateSep = findPenultimateSeparator(path, lastSep);
            const size_t start = penultimateSep + 1;
            const size_t length = lastSep - start;

            // Copy the parent directory name
            for (size_t i = 0; i < length && i < MaxSize - 1; ++i) {
                result.data[i] = path[start + i];
            }
            result.data[length < MaxSize - 1 ? length : MaxSize - 1] = '\0';

            return result;
        }
    }
}

#define CATEGORY_NAME_FROM_PATH() \
    []() { \
            static constexpr auto name = JApp::Internal::categoryNameFromPath(__FILE__); \
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
