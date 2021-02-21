// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/LibraryInit.h>

namespace nuts {
    struct LibraryInitOverride final : LibraryInit {
        LibraryInitOverride()  = default;
        ~LibraryInitOverride() = default;
    };

    namespace {
        static nuts::NutsMemoryResource nuts_memory_resource {};
        static LibraryInitOverride      nuts_library_init {};
    } // namespace

    extern "C" NUTS_API std::pmr::memory_resource* get_nuts_memory_resource() noexcept { return std::addressof(nuts_memory_resource); }
} // namespace nuts