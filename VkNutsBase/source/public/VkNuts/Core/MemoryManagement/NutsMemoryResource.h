#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

namespace nuts {
    class NutsMemoryResource final : public std::pmr::memory_resource {
      public:
        NutsMemoryResource();
        NutsMemoryResource(std::pmr::memory_resource* upStream);
        DELETE_COPY_CLASS(NutsMemoryResource)
        DELETE_MOVE_CLASS(NutsMemoryResource)
        ~NutsMemoryResource();

      private:
        std::pmr::memory_resource* mUpStream;

        void* do_allocate(std::size_t bytes, std::size_t alignment) override;
        void  do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override;
        bool  do_is_equal(const memory_resource& rhs) const noexcept override;
    };
} // namespace nuts