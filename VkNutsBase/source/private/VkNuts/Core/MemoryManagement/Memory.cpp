// clang-format off
#include <NutsPCH.h>
// clang-format on

namespace nuts {

} // namespace nuts

void* operator new(std::size_t n) throw(std::bad_alloc) {
    nuts::NutsAllocator< std::byte > alloc {};
    return reinterpret_cast< void* >(alloc.allocate(n));
}
void operator delete(void* p, size_t sz) {
    nuts::NutsAllocator< std::byte > alloc {};
    alloc.deallocate(reinterpret_cast< std::byte* >(p), sz);
}
