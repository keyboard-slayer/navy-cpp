module;

#include <cstdint>
#include <functional>
#include <optional>
#include <string_view>

export module Logger:impl;

import Traits;
import Text;

struct LogLevel {
    enum Level : uint8_t {
        debug,
        info,
        warning,
        error,
    };

    Level _level;

    constexpr LogLevel(Level level) : _level(level) {}

    constexpr std::string_view toString() {
        switch (this->_level) {
            case debug:
                return "DEBUG";
            case info:
                return "INFO";
            case warning:
                return "WARN";
            case error:
                return "ERROR";
            default:
                return "UNKNOWN";
        }
    }

    constexpr std::string_view ansiColor() {
        switch (this->_level) {
            case debug:
                return "\033[34m";
            case info:
                return "\033[32m";
            case warning:
                return "\033[33m";
            case error:
                return "\033[31m";
            default:
                return "\033[0m";
        }
    }
};

export template <traits::IOWriter W>
class Logger {
    inline static std::optional<std::reference_wrapper<W>> _writer;

public:
    static void use(W &writer) {
        _writer = std::ref(writer);
    }

    template <typename... Args>
    static void debug(std::string_view s, Args const&... args) {
        log(LogLevel::debug, s, args...);
    }

    template <typename... Args>
    static void log([[maybe_unused]] LogLevel lvl, [[maybe_unused]] std::string_view s, [[maybe_unused]] Args const&... args) {
        if (!_writer.has_value())
            return;

        fmt::formatW(_writer->get(), "{s}{s: >6} \033[0m", lvl.ansiColor(), lvl.toString());
        fmt::formatW(_writer->get(), s, args...);
        _writer->get().write('\n');
    }
};