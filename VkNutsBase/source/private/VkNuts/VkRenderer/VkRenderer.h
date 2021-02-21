#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/Shader.h>
#include <VkNuts/VkRenderer/Texture.h>

namespace nuts {
    class VkRenderer {
      public:
        STATIC_CLASS(VkRenderer)

        static void Init(vk::Device device) noexcept;
        static void Finalize() noexcept;

        static bool             LoadShader(const char* shaderPath, const char* alias) noexcept;
        static bool             CreateShader(const char* alias) noexcept;
        static bool             DestroyShader(const char* alias) noexcept;
        static vk::ShaderModule GetShader(const char* alias);

        // LoadImage is used in WindowsMacro
        // I will rename later
        static bool          Load_Image(const char* imagePath, const char* alias, ImageLoadFormat fmt) noexcept;
        static bool          CreateImage(const char* alias, uint32_t queueIndexFamily, const vk::ImageCreateInfo* imageCreateInfo,
                                         const vk::ImageViewCreateInfo* imageViewCreateInfo) noexcept;
        static bool          DestroyImage(const char* alias) noexcept;
        static vk::Image     GetVkImage(const char* alias);
        static vk::ImageView GetVkImageView(const char* alias);

        static uint32_t GetMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

        static vk::PhysicalDevice mPhysicalDevice;
      private:
        static ShaderRegistry     mShaderRegistry;
        static ImageRegistry      mImageRegistry;
    };

} // namespace nuts