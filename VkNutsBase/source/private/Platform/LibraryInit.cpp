// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/LibraryInit.h>

namespace nuts {
    struct LibraryInit final {
        LibraryInit() { std::pmr::set_default_resource(get_nuts_memory_resource()); }
        ~LibraryInit() { std::pmr::set_default_resource(std::pmr::new_delete_resource()); }
    };

    namespace {
        static nuts::NutsMemoryResource nuts_memory_resource {};
        static LibraryInit              nuts_library_init {};
    } // namespace

    extern "C" NUTS_API std::pmr::memory_resource* get_nuts_memory_resource() noexcept { return std::addressof(nuts_memory_resource); }
} // namespace nuts