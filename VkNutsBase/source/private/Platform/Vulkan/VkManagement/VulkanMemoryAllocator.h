#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/SingletonCRTP.hpp>

#if defined(NUTS_COMPILER_MSVC)
    #pragma warning(push, 4)
    #pragma warning(disable : 4127) // conditional expression is constant
    #pragma warning(disable : 4100) // unreferenced formal parameter
    #pragma warning(disable : 4189) // local variable is initialized but not referenced
    #pragma warning(disable : 4324) // structure was padded due to alignment specifier
#endif
#if defined(NUTS_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #pragma clang diagnostic ignored "-Wunused-private-field"
    #pragma clang diagnostic ignored "-Wunused-parameter"
    #pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif

#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#define VMA_STATIC_VULKAN_FUNCTIONS  0
#include <vk_mem_alloc.h>

#if defined(NUTS_COMPILER_CLANG)
    #pragma clang diagnostic pop
#endif
#if defined(NUTS_COMPILER_MSVC)
    #pragma warning(pop)
#endif

namespace nuts {

    struct Buffer {
        vk::Buffer    buffer { nullptr };
        VmaAllocation allocation { nullptr };
    };
    struct Image {
        vk::Image       image { nullptr };
        vk::ImageLayout imageLayout { vk::ImageLayout::eUndefined };
        VmaAllocation   allocation { nullptr };
    };
    struct Texture {
        vk::Image       image { nullptr };
        vk::ImageView   imageView { nullptr };
        vk::Sampler     imageSampler { nullptr };
        vk::ImageLayout imageLayout { vk::ImageLayout::eUndefined };
        VmaAllocation   allocation { nullptr };
    };
    struct AccelerationStructureKHR {
        vk::AccelerationStructureKHR accelerationStructure { nullptr };
        VmaAllocation                allocation { nullptr };
    };
    struct AllocationChunk {
        uint32_t         memoryType { 0 };
        vk::DeviceMemory deviceMemory { nullptr };
        vk::DeviceSize   offset { 0 };
        vk::DeviceSize   size { 0 };
        void*            pMappedData { nullptr };
    };

    class VulkanMemoryAllocator : public Singleton< VulkanMemoryAllocator > {
      public:
        using allocate_memory_callback = PFN_vmaAllocateDeviceMemoryFunction;
        using free_memory_callback     = PFN_vmaFreeDeviceMemoryFunction;

      protected:
        VulkanMemoryAllocator();

      public:
        ~VulkanMemoryAllocator();
        DELETE_COPY_CLASS(VulkanMemoryAllocator)
        DELETE_MOVE_CLASS(VulkanMemoryAllocator)

        NUTS_API [[nodiscard]] bool init(vk::Instance instance, vk::Device device, vk::PhysicalDevice physicalDevice, allocate_memory_callback = nullptr,
                                         free_memory_callback = nullptr) noexcept;
        NUTS_API void               finalize() noexcept;

        // Buffer
        NUTS_API [[nodiscard]] inline Buffer createBuffer(const vk::BufferCreateInfo& bufferCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        NUTS_API [[nodiscard]] inline Buffer createBuffer(const vk::DeviceSize bufferSize, const vk::BufferUsageFlags bufferUsage,
                                                          const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        NUTS_API [[nodiscard]] inline Buffer createBuffer(const vk::BufferCreateInfo& bufferCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags,
                                                          const void* data) noexcept;
        NUTS_API [[nodiscard]] inline Buffer createBuffer(const vk::DeviceSize bufferSize, const void* data, const vk::BufferUsageFlags bufferUsage,
                                                          const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;

        NUTS_API [[nodiscard]] inline bool copyDataToBuffer(Buffer buffer, vk::DeviceSize bufferSize, const void* data) const noexcept;
        NUTS_API [[nodiscard]] inline bool copyDataFromBuffer(const Buffer& buffer, vk::DeviceSize dataSize, void* data) const noexcept;

        // Image
        NUTS_API [[nodiscard]] inline Image createImage(const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;

        NUTS_API [[nodiscard]] inline Image createImage(vk::CommandBuffer commandBuffer, const vk::ImageCreateInfo& imageCreateInfo,
                                                        const vk::MemoryPropertyFlags memoryPropertyFlags, vk::ImageLayout imageLayout) noexcept;

        NUTS_API [[nodiscard]] inline Texture createTexture(const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags,
                                                            const vk::ImageViewCreateInfo& imageViewCreateInfo) noexcept;
        NUTS_API [[nodiscard]] inline Texture createTexture(const Image& image, const vk::ImageViewCreateInfo& imageViewCreateInfo) noexcept;
        NUTS_API [[nodiscard]] inline Texture createTexture(vk::CommandBuffer commandBuffer, const vk::ImageCreateInfo& imageCreateInfo,
                                                            const vk::MemoryPropertyFlags memoryPropertyFlags, vk::ImageLayout imageLayout,
                                                            const vk::ImageViewCreateInfo& imageViewCreateInfo) noexcept;

        // Destroy vulkan objects
        NUTS_API inline void destroyBuffer(Buffer& buffer) noexcept;
        NUTS_API inline void destroyImage(Image& image) noexcept;
        NUTS_API inline void destroyTexture(Texture& texture) noexcept;

        // Obtain VkDeviceMemory
        template < class VkAllocatedType >
        NUTS_API [[nodiscard]] inline AllocationChunk getAllocationMemory(const VkAllocatedType& type) const noexcept;

        NUTS_API [[nodiscard]] inline void* mapAllocation(VmaAllocation allocation) noexcept;
        NUTS_API [[nodiscard]] inline void  unmapAllocation(VmaAllocation allocation) noexcept;

      private:
        [[nodiscard]] inline constexpr static VmaMemoryUsage VkMemFlagsToVmaMemoryUsage(vk::MemoryPropertyFlags flags) noexcept;

        vk::Device   mDevice { nullptr };
        VmaAllocator mAllocator { nullptr };
    };
} // namespace nuts

#if !defined(NUTS_BUILD_DLL)
extern template nuts::AllocationChunk NUTS_API nuts::VulkanMemoryAllocator::getAllocationMemory< nuts::Buffer >(const Buffer& buffer) const noexcept;
extern template nuts::AllocationChunk NUTS_API nuts::VulkanMemoryAllocator::getAllocationMemory< nuts::Image >(const Image& buffer) const noexcept;
extern template nuts::AllocationChunk NUTS_API nuts::VulkanMemoryAllocator::getAllocationMemory< nuts::Texture >(const Texture& buffer) const noexcept;
#endif // !defined(NUTS_BUILD_DLL)
