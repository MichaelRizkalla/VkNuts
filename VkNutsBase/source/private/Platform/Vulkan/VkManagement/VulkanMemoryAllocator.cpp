// clang-format off
#include <NutsPCH.h>
// clang-format on

#define VMA_IMPLEMENTATION
#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>
#include <Platform/Vulkan/VulkanImage.h>

namespace nuts {
    VulkanMemoryAllocator::VulkanMemoryAllocator()  = default;
    VulkanMemoryAllocator::~VulkanMemoryAllocator() = default;

    constexpr VmaMemoryUsage VulkanMemoryAllocator::VkMemFlagsToVmaMemoryUsage(vk::MemoryPropertyFlags flags) noexcept {
        if ((flags & vk::MemoryPropertyFlagBits::eDeviceLocal) == vk::MemoryPropertyFlagBits::eDeviceLocal)
            return VMA_MEMORY_USAGE_GPU_ONLY;
        else if ((flags & (vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)) ==
                 (vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent))
            return VMA_MEMORY_USAGE_CPU_ONLY;
        else if ((flags & vk::MemoryPropertyFlagBits::eHostVisible) == vk::MemoryPropertyFlagBits::eHostVisible)
            return VMA_MEMORY_USAGE_CPU_TO_GPU;

        return VMA_MEMORY_USAGE_UNKNOWN;
    }

    bool VulkanMemoryAllocator::init(vk::Instance instance, vk::Device device, vk::PhysicalDevice physicalDevice, allocate_memory_callback allocMemCallback,
                                     free_memory_callback freeMemCallback) noexcept {
        mDevice = device;

        VmaDeviceMemoryCallbacks devMemCallback {};
        devMemCallback.pfnAllocate = allocMemCallback;
        devMemCallback.pfnFree     = freeMemCallback;

        VmaVulkanFunctions vulkanFunctions {};
        vulkanFunctions.vkAllocateMemory                        = VULKAN_HPP_DEFAULT_DISPATCHER.vkAllocateMemory;
        vulkanFunctions.vkBindBufferMemory                      = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindBufferMemory;
        vulkanFunctions.vkBindBufferMemory2KHR                  = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindBufferMemory2;
        vulkanFunctions.vkBindImageMemory                       = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindImageMemory;
        vulkanFunctions.vkBindImageMemory2KHR                   = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindImageMemory2;
        vulkanFunctions.vkCmdCopyBuffer                         = VULKAN_HPP_DEFAULT_DISPATCHER.vkCmdCopyBuffer;
        vulkanFunctions.vkCreateBuffer                          = VULKAN_HPP_DEFAULT_DISPATCHER.vkCreateBuffer;
        vulkanFunctions.vkCreateImage                           = VULKAN_HPP_DEFAULT_DISPATCHER.vkCreateImage;
        vulkanFunctions.vkDestroyBuffer                         = VULKAN_HPP_DEFAULT_DISPATCHER.vkDestroyBuffer;
        vulkanFunctions.vkDestroyImage                          = VULKAN_HPP_DEFAULT_DISPATCHER.vkDestroyImage;
        vulkanFunctions.vkFlushMappedMemoryRanges               = VULKAN_HPP_DEFAULT_DISPATCHER.vkFlushMappedMemoryRanges;
        vulkanFunctions.vkFreeMemory                            = VULKAN_HPP_DEFAULT_DISPATCHER.vkFreeMemory;
        vulkanFunctions.vkGetBufferMemoryRequirements           = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetBufferMemoryRequirements;
        vulkanFunctions.vkGetBufferMemoryRequirements2KHR       = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetBufferMemoryRequirements2;
        vulkanFunctions.vkGetImageMemoryRequirements            = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetImageMemoryRequirements;
        vulkanFunctions.vkGetImageMemoryRequirements2KHR        = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetImageMemoryRequirements2;
        vulkanFunctions.vkGetPhysicalDeviceMemoryProperties     = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceMemoryProperties;
        vulkanFunctions.vkGetPhysicalDeviceMemoryProperties2KHR = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceMemoryProperties2;
        vulkanFunctions.vkGetPhysicalDeviceProperties           = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceProperties;
        vulkanFunctions.vkInvalidateMappedMemoryRanges          = VULKAN_HPP_DEFAULT_DISPATCHER.vkInvalidateMappedMemoryRanges;
        vulkanFunctions.vkMapMemory                             = VULKAN_HPP_DEFAULT_DISPATCHER.vkMapMemory;
        vulkanFunctions.vkUnmapMemory                           = VULKAN_HPP_DEFAULT_DISPATCHER.vkUnmapMemory;

        VmaAllocatorCreateInfo allocatorCreateInfo {};
        allocatorCreateInfo.flags                       = VMA_ALLOCATOR_CREATE_FLAG_BITS_MAX_ENUM;
        allocatorCreateInfo.physicalDevice              = physicalDevice;
        allocatorCreateInfo.device                      = device;
        allocatorCreateInfo.preferredLargeHeapBlockSize = 0;
        allocatorCreateInfo.pAllocationCallbacks        = nullptr;
        allocatorCreateInfo.pDeviceMemoryCallbacks      = &devMemCallback;
        allocatorCreateInfo.frameInUseCount             = 0;
        allocatorCreateInfo.pHeapSizeLimit              = nullptr;
        allocatorCreateInfo.pVulkanFunctions            = &vulkanFunctions;
        allocatorCreateInfo.pRecordSettings             = nullptr;
        allocatorCreateInfo.instance                    = instance;
        allocatorCreateInfo.vulkanApiVersion            = 0;

        if (static_cast< VkResult >(vk::Result::eSuccess) != vmaCreateAllocator(&allocatorCreateInfo, &mAllocator)) { return false; }
        return true;
    }
    void VulkanMemoryAllocator::finalize() noexcept { vmaDestroyAllocator(mAllocator); }

    Buffer VulkanMemoryAllocator::createBuffer(const vk::BufferCreateInfo& bufferCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        VmaAllocationCreateInfo allocationCreateInfo = {};
        allocationCreateInfo.usage                   = VulkanMemoryAllocator::VkMemFlagsToVmaMemoryUsage(memoryPropertyFlags);

        Buffer   buffer;
        VkBuffer buf = buffer.buffer;
        if (VK_SUCCESS !=
            vmaCreateBuffer(mAllocator, reinterpret_cast< const VkBufferCreateInfo* >(&bufferCreateInfo), &allocationCreateInfo, &buf, &buffer.allocation, nullptr)) {
            return {};
        }
        return buffer;
    }
    Buffer VulkanMemoryAllocator::createBuffer(const vk::DeviceSize bufferSize, const vk::BufferUsageFlags bufferUsage,
                                               const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        vk::BufferCreateInfo bufferCreateInfo {};
        bufferCreateInfo.size  = bufferSize;
        bufferCreateInfo.usage = bufferUsage | vk::BufferUsageFlagBits::eTransferDst;
        return createBuffer(bufferCreateInfo, memoryPropertyFlags);
    }
    Buffer VulkanMemoryAllocator::createBuffer(const vk::BufferCreateInfo& bufferCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags,
                                               const void* data) noexcept {
        auto buffer = createBuffer(bufferCreateInfo, memoryPropertyFlags);

        if (copyDataToBuffer(buffer, bufferCreateInfo.size, data)) {
            destroyBuffer(buffer);
            return {};
        }

        return buffer;
    }
    Buffer VulkanMemoryAllocator::createBuffer(const vk::DeviceSize bufferSize, const void* data, const vk::BufferUsageFlags bufferUsage,
                                               const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        auto buffer = createBuffer(bufferSize, bufferUsage, memoryPropertyFlags);

        if (copyDataToBuffer(buffer, bufferSize, data)) {
            destroyBuffer(buffer);
            return {};
        }

        return buffer;
    }

    bool VulkanMemoryAllocator::copyDataToBuffer(Buffer buffer, vk::DeviceSize bufferSize, const void* data) const noexcept {
        void* mappedData = nullptr;
        if (VK_SUCCESS != vmaMapMemory(mAllocator, buffer.allocation, &mappedData)) { return false; }
        std::memcpy(mappedData, data, bufferSize);
        vmaUnmapMemory(mAllocator, buffer.allocation);
        return true;
    }
    bool VulkanMemoryAllocator::copyDataFromBuffer(const Buffer& buffer, vk::DeviceSize dataSize, void* data) const noexcept {
        void* mappedData = nullptr;
        if (VK_SUCCESS != vmaMapMemory(mAllocator, buffer.allocation, &mappedData)) { return false; }
        std::memcpy(data, mappedData, dataSize);
        vmaUnmapMemory(mAllocator, buffer.allocation);
        return true;
    }

    Image VulkanMemoryAllocator::createImage(const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        VmaAllocationCreateInfo allocationCreateInfo = {};
        allocationCreateInfo.usage                   = VulkanMemoryAllocator::VkMemFlagsToVmaMemoryUsage(memoryPropertyFlags);

        Image   image;
        VkImage img = image.image;
        if (VK_SUCCESS !=
            vmaCreateImage(mAllocator, reinterpret_cast< const VkImageCreateInfo* >(&imageCreateInfo), &allocationCreateInfo, &img, &image.allocation, nullptr)) {
            return {};
        }
        image.imageLayout = imageCreateInfo.initialLayout;
        return image;
    }
    Image VulkanMemoryAllocator::createImage(vk::CommandBuffer commandBuffer, const vk::ImageCreateInfo& imageCreateInfo,
                                             const vk::MemoryPropertyFlags memoryPropertyFlags, vk::ImageLayout imageLayout) noexcept {
        auto image = createImage(imageCreateInfo, memoryPropertyFlags);
        if (!image.image) { return image; }
        VulkanImage::changeImageLayout(commandBuffer, image.image, image.imageLayout, imageLayout);
        image.imageLayout = imageLayout;
        return image;
    }

    Texture VulkanMemoryAllocator::createTexture(const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags,
                                                 const vk::ImageViewCreateInfo& imageViewCreateInfo) noexcept {
        auto image = createImage(imageCreateInfo, memoryPropertyFlags);
        if (!image.image) { return {}; }

        return createTexture(image, imageViewCreateInfo);
    }
    Texture VulkanMemoryAllocator::createTexture(const Image& image, const vk::ImageViewCreateInfo& imageViewCreateInfo) noexcept {
        assert(imageViewCreateInfo.image == image.image);
        Texture texture;
        texture.image       = image.image;
        texture.allocation  = image.allocation;
        texture.imageLayout = vk::ImageLayout::eUndefined;

        if (vk::Result::eSuccess != mDevice.createImageView(&imageViewCreateInfo, nullptr, &texture.imageView)) { return {}; }

        return texture;
    }
    Texture VulkanMemoryAllocator::createTexture(vk::CommandBuffer commandBuffer, const vk::ImageCreateInfo& imageCreateInfo,
                                                 const vk::MemoryPropertyFlags memoryPropertyFlags, vk::ImageLayout imageLayout,
                                                 const vk::ImageViewCreateInfo& imageViewCreateInfo) noexcept {
        auto image = createImage(commandBuffer, imageCreateInfo, memoryPropertyFlags, imageLayout);
        if (!image.image) { return {}; }

        return createTexture(image, imageViewCreateInfo);
    }

    void VulkanMemoryAllocator::destroyBuffer(Buffer& buffer) noexcept {
        if (buffer.buffer) { vmaDestroyBuffer(mAllocator, buffer.buffer, buffer.allocation); }
        buffer.buffer     = nullptr;
        buffer.allocation = nullptr;
    }
    void VulkanMemoryAllocator::destroyImage(Image& image) noexcept {
        if (image.image) { vmaDestroyImage(mAllocator, image.image, image.allocation); }
        image.image       = nullptr;
        image.allocation  = nullptr;
        image.imageLayout = vk::ImageLayout::eUndefined;
    }
    void VulkanMemoryAllocator::destroyTexture(Texture& texture) noexcept {
        if (texture.imageView) { mDevice.destroyImageView(texture.imageView); }
        texture.imageView = nullptr;
        if (texture.image) { vmaDestroyImage(mAllocator, texture.image, texture.allocation); }
        texture.image       = nullptr;
        texture.allocation  = nullptr;
        texture.imageLayout = vk::ImageLayout::eUndefined;
    }
} // namespace nuts
