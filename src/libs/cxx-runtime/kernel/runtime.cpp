#include <cstdlib>
#include <cxxabi.h>

import Arch;

using namespace __cxxabiv1;

extern "C" int __cxa_guard_acquire(__guard* g) {
    return !*reinterpret_cast<char*>(g);
}

extern "C" void __cxa_guard_release(__guard* g) {
    *reinterpret_cast<char*>(g) = 1;
}

extern "C" void __cxa_pure_virtual() {
    while (1)
        ;
}

namespace std {
void terminate() noexcept {
    arch::getDefaultWriter().write("Fatal error: std::terminate() called!\n");
    for (;;) {
        asm volatile("hlt");
    }
}
} // namespace std
