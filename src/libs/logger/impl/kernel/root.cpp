module;

#include <type_traits>

export module Logger:embed;

import :impl;
import Arch;

export using Log = Logger<std::remove_reference_t<decltype(arch::getDefaultWriter())>>;