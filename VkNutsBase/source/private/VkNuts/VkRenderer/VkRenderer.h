#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/Shader.h>
#include <VkNuts/VkRenderer/Texture.h>
#include <Platform/Vulkan/VulkanHelper.h>
#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>
#include <Platform/Vulkan/VkManagement/VkSamplerManager.h>

namespace nuts {
    class VkRenderer {
      public:
        STATIC_CLASS(VkRenderer)

        static void init(vk::Instance instance, vk::Device device, vk::PhysicalDevice physicalDevice) noexcept;
        static void finalize() noexcept;

        static bool             loadShader(const char* shaderPath, const char* alias) noexcept;
        static bool             createShader(const char* alias) noexcept;
        static bool             destroyShader(const char* alias) noexcept;
        static vk::ShaderModule getShader(const char* alias);

        static bool          loadImage(const char* imagePath, const char* alias, ImageLoadFormat fmt) noexcept;
        static bool          createImage(const char* alias, const vk::ImageCreateInfo* imageCreateInfo, const vk::ImageViewCreateInfo* imageViewCreateInfo,
                                         const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        static bool          destroyImage(const char* alias) noexcept;
        static vk::Image     getVkImage(const char* alias);
        static vk::ImageView getVkImageView(const char* alias);

        static uint32_t getMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

      private:
        static vk::Instance          mInstance;
        static vk::PhysicalDevice    mPhysicalDevice;
        static vk::Device            mDevice;
        static ShaderRegistry        mShaderRegistry;
        static ImageRegistry         mImageRegistry;
        static VulkanMemoryAllocator mAllocator;
        static VkSamplerManager      mSamplerManager;
    };

} // namespace nuts