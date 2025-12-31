module;

#include <cstddef>
#include <string_view>

export module Traits;

export namespace traits {
struct IOWriter {
    size_t write(std::string_view const s) const {
        size_t count = 0;
        for (char const chr: s)
             count += this->write(chr);
        return count;
    }

    virtual size_t write(char c) const = 0;
};
}; // namespace traits