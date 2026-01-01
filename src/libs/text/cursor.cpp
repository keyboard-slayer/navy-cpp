module;

#include <algorithm>
#include <cstddef>
#include <optional>
#include <span>
#include <string_view>

export module Text:cursor;

export class Cursor {
    std::string_view _str;
    size_t pos = 0;

public:
    explicit Cursor(std::string_view const& str) : _str(str) {}

    constexpr std::size_t rem() const {
        return _str.size() - pos;
    }

    constexpr std::optional<char const> peek(size_t off = 0) const {
        if (off > rem())
            return std::nullopt;
        return _str.at(pos + off);
    }

    constexpr char const& next() {
        if (this->ended())
            return _str.at(pos);
        return _str.at(pos++);
    }

    bool skip(char const el) {
        if (this->peek().value_or(0) != el)
            return false;

        this->next();
        return true;
    }

    bool skip(std::string_view const& slice) {
        return std::none_of(
            slice.begin(),
            slice.end(),
            [this](char const el) {
                return !this->skip(el);
            });
    }

    void eat(bool (*fn)(char el), std::span<char> &buffer) {
        if (this->ended())
            return;

        std::optional<char> current;
        size_t idx = 0;

        do {
            current = this->next();
            if (!fn(current.value()))
                break;
            buffer[idx++] = current.value();
        } while (!this->ended());
    }

    bool ended() const {
        return rem() == 0;
    }
};