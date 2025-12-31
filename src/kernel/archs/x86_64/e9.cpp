module;

#include <cstdint>
#include <cstddef>

export module Arch:e9;

import :port;
import Traits;

export namespace E9 {
struct E9 : traits::IOWriter {
    size_t write(char c) const override {
        static const Port<uint8_t> _port{0xE9};
        _port.write(c);
        return 1;
    }
};
} // namespace E9