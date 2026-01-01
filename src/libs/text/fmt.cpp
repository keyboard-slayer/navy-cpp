module;

#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <string_view>

#include <ctype.h>

export module Text:fmt;

import Traits;

import :cursor;

export namespace fmt {
char *int2str(char* buf, int64_t n, char base, bool upper) {
    if (n < 0) {
        *buf++ = '-';
        return int2str(buf, n, base, upper);
    }

    if (n >= base) {
        buf = int2str(buf, n / base, base, upper);
    }

    *buf++ = ((n % base) > 9) ? ((n % base) - 10) + (upper ? 'A' : 'a') : (n % base) + '0';
    *buf = '\0';
    return buf;
}

template <size_t N>
int64_t str2int(std::array<char, N> const& str) {
    int64_t sign = 1;
    int64_t result = 0;
    bool skip = false;

    if (str.at(0) == '-') {
        sign = -1;
        skip = true;
    }

    if (str.at(0) == '+')
        skip = true;

    size_t p = skip ? 1 : 0;
    while (isdigit(str.at(p))) {
        result *= 10;
        result += (str.at(p++) - '0');
    }

    return sign * result;
}

struct Placeholder {
    enum Specifier {
        undefined,
        lower_hexa, // x
        upper_hexa, // X
        string,     // s
        decimal,    // d
        character,  // c
        optional,   // ?
        address,    // *
        any,        // any
    } spec;

    char fill;

    enum Alignment {
        none,
        left,   // <
        center, // ^
        right,  // >
    } align;

    size_t width;

    static std::optional<Placeholder> fromCursor(Cursor& cur) {
        Placeholder self{};

        if (cur.skip(std::string_view("any")))
            self.spec = any;
        else {
            switch (cur.next()) {
            case 'x':
                self.spec = lower_hexa;
                break;
            case 'X':
                self.spec = upper_hexa;
                break;
            case 's':
                self.spec = string;
                break;
            case 'd':
                self.spec = decimal;
                break;
            case 'c':
                self.spec = character;
                break;
            case '?':
                self.spec = optional;
                break;
            case '*':
                self.spec = address;
                break;
            default:
                self.spec = undefined;
                break;
            }
        }

        if (cur.ended())
            return self;

        if (!cur.skip(':'))
            return {};

        self.fill = cur.next();

        if (cur.ended())
            return {};

        if (!isdigit(cur.peek().value())) {
            switch (cur.peek().value()) {
            case '<':
                self.align = left;
                break;
            case '^':
                self.align = center;
                break;
            case '>':
                self.align = right;
                break;
            default:
                return {};
            }
        } else
            return {};

        cur.next();

        std::array<char, 9> width{0};
        std::span<char> buffer(width);

        cur.eat(
            [](char c) {
                return isdigit(c) == 1;
            },
            buffer
        );

        self.width = str2int(width);

        return self;
    }
};

template <traits::IOWriter W>
void writeValueW(W& w, Placeholder fmt, std::string_view value) {
    if (fmt.align == Placeholder::left)
        for (size_t i = 0; i < fmt.width - value.size(); ++i)
            w.write(fmt.fill);

    if (fmt.spec == Placeholder::any) {
        w.write('\"');
        w.write(value);
        w.write('\"');
    } else if (fmt.spec != Placeholder::undefined && fmt.spec != Placeholder::string) [[unlikely]] {
        w.write("??");
    } else {
        w.write(value);
    }
    if (fmt.align == Placeholder::right)
        for (size_t i = 0; i < fmt.width - value.size(); ++i)
            w.write(fmt.fill);
}

template <traits::IOWriter W>
void writeValueW(W& w, Placeholder fmt, uint64_t value) {
    char buf[32];
    switch (fmt.spec) {
    case Placeholder::any:
    case Placeholder::decimal:
        int2str(buf, value, 10, false);
        break;
    case Placeholder::lower_hexa:
        int2str(buf, value, 16, false);
        break;
    case Placeholder::upper_hexa:
        int2str(buf, value, 16, true);
        break;
    case Placeholder::character:
        buf[0] = (char)value;
        buf[1] = 0;
    default:
        buf[0] = '?';
        buf[1] = '?';
        buf[2] = 0;
        return;
    }

    if (fmt.align == Placeholder::left)
        for (size_t i = 0; i < fmt.width - std::char_traits<char>::length(buf); ++i)
            w.write(fmt.fill);

    w.write(buf);

    if (fmt.align == Placeholder::right)
        for (size_t i = 0; i < fmt.width - std::char_traits<char>::length(buf); ++i)
            w.write(fmt.fill);
}

template <typename T, traits::IOWriter W>
void formatWValue(W& w, std::string_view& fstr, T const& value) {
    while (!fstr.empty()) {
        if (fstr.front() == '{') {
            fstr.remove_prefix(1);

            size_t const close_pos = fstr.find('}');
            if (close_pos == std::string_view::npos) [[unlikely]] {
                w.write("??");
                return;
            }

            std::string_view const f = fstr.substr(0, close_pos);
            fstr.remove_prefix(close_pos + 1);

            Cursor fmtCursor{f};
            auto holder = Placeholder::fromCursor(fmtCursor);
            if (!holder.has_value()) [[unlikely]] {
                w.write("??");
                return;
            }
            writeValueW(w, holder.value(), value);
            return;
        }

        w.write(fstr.front());
        fstr.remove_prefix(1);
    }
}

template <traits::IOWriter W, typename... Args>
void formatW(W& w, std::string_view s, Args const&... args) {
    (formatWValue(w, s, args), ...);

    if (!s.empty())
        w.write(s);
}
}; // namespace fmt