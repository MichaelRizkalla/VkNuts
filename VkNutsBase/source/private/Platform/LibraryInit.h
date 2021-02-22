#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

namespace nuts {
    extern "C" NUTS_API std::pmr::memory_resource* get_nuts_memory_resource() noexcept;
} // namespace nuts