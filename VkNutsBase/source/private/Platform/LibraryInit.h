#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

namespace nuts {
    extern "C" NUTS_API std::pmr::memory_resource* get_nuts_memory_resource() noexcept;

    // Inheriting from this struct could cause undefined behaviour
    struct LibraryInit {
      protected:
        LibraryInit() { std::pmr::set_default_resource(get_nuts_memory_resource()); }
        virtual ~LibraryInit() { std::pmr::set_default_resource(std::pmr::new_delete_resource()); }
    };
} // namespace nuts