module;

#include <concepts>
#include <cstddef>
#include <string_view>

export module Traits;

export namespace traits {
template <typename T>
concept IOWriter = requires(T &t, char c, std::string_view const &s) {
    { t.write(c) } -> std::same_as<size_t>;
    { t.write(s) } -> std::same_as<size_t>;
};
}; // namespace traits
