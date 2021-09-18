// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/MemoryManagement/NutsMemoryResource.h>

namespace nuts {

    NutsMemoryResource::NutsMemoryResource() : mUpStream(std::pmr::new_delete_resource()) {}

    NutsMemoryResource::NutsMemoryResource(std::pmr::memory_resource* upStream) : mUpStream(upStream) {}

    NutsMemoryResource::~NutsMemoryResource() = default;

    void* NutsMemoryResource::do_allocate(std::size_t bytes, std::size_t alignment) {
        NUTS_ALLOCATION_TRACE_IF_INIT("NutsMemoryResource (Allocation Attempt): Size: {}, Alignment: {} ...", bytes, alignment);
        auto result = mUpStream->allocate(bytes, alignment);
        NUTS_ALLOCATION_TRACE_IF_INIT("NutsMemoryResource (Allocation Successful): Address: {} ...", result);
        return result;
    }

    void NutsMemoryResource::do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) {
        NUTS_ALLOCATION_TRACE_IF_INIT("NutsMemoryResource (Deallocation): Address: {}, Size: {}, Alignment: {} ...", ptr, bytes, alignment);
        mUpStream->deallocate(ptr, bytes, alignment);
    }

    bool NutsMemoryResource::do_is_equal(const memory_resource& rhs) const noexcept { return this == &rhs; }

} // namespace nuts
