// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/MemoryManagement/NutsMemoryResource.h>

namespace nuts {

    NutsMemoryResource::NutsMemoryResource() : mUpStream(std::pmr::new_delete_resource()) {}

    NutsMemoryResource::NutsMemoryResource(std::pmr::memory_resource* upStream) : mUpStream(upStream) {}

    NutsMemoryResource::~NutsMemoryResource() = default;

    void* NutsMemoryResource::do_allocate(std::size_t bytes, std::size_t alignment) {
        NUTS_LOG_WIN(auto stdOutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE));
        NUTS_LOG_WIN(::SetConsoleTextAttribute(stdOutHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY));
        NUTS_LOG(std::cout << "NutsMemoryResource (Allocation Attempt): Size: " << bytes << ", Alignment: " << alignment << " ...\n");
        auto result = mUpStream->allocate(bytes, alignment);
        NUTS_LOG(std::cout << "NutsMemoryResource (Allocation Successful): Address: " << result << " ...\n");
        NUTS_LOG_WIN(::SetConsoleTextAttribute(stdOutHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY));
        return result;
    }

    void NutsMemoryResource::do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) {
        NUTS_LOG_WIN(auto stdOutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE));
        NUTS_LOG_WIN(::SetConsoleTextAttribute(stdOutHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY));
        NUTS_LOG(std::cout << "NutsMemoryResource (Deallocation): Address: " << ptr << ", Size: " << bytes << ", Alignment: " << alignment << " ...\n");
        NUTS_LOG_WIN(::SetConsoleTextAttribute(stdOutHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY));
        mUpStream->deallocate(ptr, bytes, alignment);
    }

    bool NutsMemoryResource::do_is_equal(const memory_resource& rhs) const noexcept { return this == &rhs; }

} // namespace nuts