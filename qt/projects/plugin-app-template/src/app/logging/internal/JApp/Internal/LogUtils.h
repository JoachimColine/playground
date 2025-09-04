#pragma once

namespace JApp {
    namespace Internal {
		namespace LogUtils {
            namespace detail {
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

                constexpr size_t findAntepenultimateSeparator(const char* path, size_t penultimateSep) {
                    size_t antepenultimateSep = 0;

                    for (size_t i = 0; i < penultimateSep; ++i) {
                        if (path[i] == '/' || path[i] == '\\') {
                            antepenultimateSep = i;
                        }
                    }

                    return antepenultimateSep;
                }

                constexpr bool strEquals(const char* str1, const char* str2, size_t len) {
                    for (size_t i = 0; i < len; ++i) {
                        if (str1[i] != str2[i] || str1[i] == '\0' || str2[i] == '\0') {
                            return false;
                        }
                    }
                    return true;
                }

                template <size_t N>
                struct StringLiteral {
                    char data[N];
                    constexpr StringLiteral() : data{} {}
                    constexpr operator const char*() const {
                        return data;
                    }
                };
            }

			template <size_t MaxSize = 64>
			constexpr auto categoryNameFromPath(const char* path) {
                detail::StringLiteral<MaxSize> result = {};

                const size_t lastSep = detail::findLastSeparator(path);
				if (lastSep == 0) {
					result.data[0] = '.';
					result.data[1] = '\0';
					return result;
				}

                const size_t penultimateSep = detail::findPenultimateSeparator(path, lastSep);
				const size_t start = penultimateSep + 1;
				const size_t length = lastSep - start;

                const bool isSrc = length == 3 &&
                                   detail::strEquals(path + start, "src", 3);

                if (isSrc) {
                    const size_t antepenultimateSep = detail::findAntepenultimateSeparator(path, penultimateSep);
                    const size_t grandparentStart = antepenultimateSep + 1;
                    const size_t grandparentLength = penultimateSep - grandparentStart;

                    for (size_t i = 0; i < grandparentLength && i < MaxSize - 1; ++i) {
                        result.data[i] = path[grandparentStart + i];
                    }
                    result.data[grandparentLength < MaxSize - 1 ? grandparentLength : MaxSize - 1] = '\0';
                } else {
                    for (size_t i = 0; i < length && i < MaxSize - 1; ++i) {
                        result.data[i] = path[start + i];
                    }
                    result.data[length < MaxSize - 1 ? length : MaxSize - 1] = '\0';
                }

				return result;
			}
		}
    }
}
