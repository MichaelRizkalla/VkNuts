#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Registry/Registry.h>
#include <VkNuts/VkRenderer/Module/Texture/Texture.h>

namespace nuts {

    struct TextureRegistryInitializer final : RegistryInitializer {
        vk::Device             mDefaultDevice;
        VulkanMemoryAllocator* mAllocator;
    };

    class TextureRegistry final : public Registry< TextureAttachment > {
      public:
        void               init(const RegistryInitializer* const init) noexcept override;
        [[nodiscard]] bool createImage(const char* alias, const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        [[nodiscard]] bool createTexture(const char* alias, const vk::ImageCreateInfo& imageCreateInfo, const vk::ImageViewCreateInfo& imageViewCreateInfo,
                                         const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        [[nodiscard]] bool destroyTexture(const char* alias) noexcept;
        void               setSampler(const char* alias, vk::Sampler sampler);

      private:
        vk::Device             mDefaultDevice;
        VulkanMemoryAllocator* mAllocator;
    };

} // namespace nuts
