export module Arch;

import :e9;
import Traits;

export namespace arch {
traits::IOWriter& getDefaultWriter() {
    static E9::E9 writer{};
    return writer;
}
} // namespace arch
