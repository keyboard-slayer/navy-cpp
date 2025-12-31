module;

#include <concepts>
#include <cstdint>

export module Arch:port;

export template <std::unsigned_integral U>
struct Port {
    uint16_t _port;

    explicit Port(uint16_t const port) : _port(port) {}

    void write(U value) const;

    U read() const;
};

template <>
void Port<uint8_t>::write(uint8_t value) const {
    asm volatile("outb %0, %1"
                 :
                 : "a"(value), "Nd"(this->_port));
}

template <>
uint8_t Port<uint8_t>::read() const {
    uint8_t value;
    asm volatile("inb %1, %0"
                 : "=a"(value)
                 : "Nd"(this->_port));
    return value;
}
