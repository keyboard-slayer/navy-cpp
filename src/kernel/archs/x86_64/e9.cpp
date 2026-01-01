module;

#include <algorithm>
#include <cstdint>
#include <cstddef>
#include <string_view>

export module Arch:e9;

import :port;
import Traits;

export namespace e9 {
struct E9 {
    Port<uint8_t> _port{0xE9};

    size_t write(char c) const {
        _port.write(c);
        return 1;
    }

    size_t write(std::string_view const& s) const {
        size_t count;
        std::for_each(s.begin(), s.end(), [this, &count](char const chr) {
            count += this->write(chr);
        });
        return count;
    }
};
} // namespace e9