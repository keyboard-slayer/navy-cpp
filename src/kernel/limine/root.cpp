module;

#include <array>
#include <limine/limine.h>

export module Loader;

import Text;
import Arch;
import Logger;

namespace {
[[gnu::used, gnu::section(".limine_requests")]]
std::array<uint64_t, 3> volatile limine_base_revision = LIMINE_BASE_REVISION(4);

[[gnu::used, gnu::section(".limine_requests_start")]]
std::array<uint64_t, 4> volatile limine_requests_start_marker = LIMINE_REQUESTS_START_MARKER;

[[gnu::used, gnu::section(".limine_requests_end")]]
std::array<uint64_t, 4> volatile limine_requests_end = LIMINE_REQUESTS_END_MARKER;
} // namespace

extern "C" [[noreturn]] void limine_entry() {
    auto& e9 = arch::getDefaultWriter();
    Log::use(e9);
    Log::debug("Hello, World !");
    for (;;) {}
}
