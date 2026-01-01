export module Arch;

import :e9;
import Traits;

export namespace arch {
e9::E9& getDefaultWriter() {
    static e9::E9 writer{};
    return writer;
}
} // namespace arch
