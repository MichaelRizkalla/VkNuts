#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Generic/ImageLoadFormat.h>
#include <Utilities/Container.hpp>
#include <VkNuts/Core/Attachment/VulkanAttachment.h>
#include <VkNuts/VkRenderer/Module/Image/Image.h>
#include <Platform/Vulkan/VulkanHelper.h>
#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>

namespace nuts {

    struct TextureAttachment final : public VulkanAttachment< vk::ImageView, Texture > {

        struct TextureAttachmentData {
            const ImageAttachment* const imageAttachment;
            vk::ImageCreateInfo          mImageCreateInfo;
            vk::ImageViewCreateInfo      mImageViewCreateInfo;
            Texture                      mTexture;
        };

        using attachment_data = TextureAttachmentData;

        [[nodiscard]] explicit TextureAttachment(const char* name, VulkanMemoryAllocator* vulkanMemoryAllocator, const ImageAttachment* const imageAttachment);
        ~TextureAttachment();

        [[nodiscard]] bool onLoad() noexcept override;
        bool               onUnload() noexcept override;
        [[nodiscard]] bool createImage(const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        [[nodiscard]] bool createTexture(const vk::ImageViewCreateInfo& imageViewCreateInfo) noexcept;
        [[nodiscard]] bool createTexture(const vk::ImageCreateInfo& imageCreateInfo, const vk::ImageViewCreateInfo& imageViewCreateInfo,
                                         const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        void               destroyTexture() noexcept;
        void               setSampler(vk::Sampler sampler) noexcept;
        [[nodiscard]] bool changeImageLayout(const vk::CommandBuffer& commandBuffer, vk::ImageLayout newImageLayout) noexcept;
        // bool ChangeImageQueueFamilyIndex(const vk::CommandBuffer& commandBuffer, uint32_t newQueueFamilyIndex) noexcept;

        [[nodiscard]] bool isVkImageCreated() const noexcept;
        [[nodiscard]] bool isVkTextureCreated() const noexcept;

        attachment_data                            getData() const noexcept;
        [[nodiscard]] const ImageAttachment* const getImageAttachment() const noexcept;
        [[nodiscard]] vk::Image                    getVkImage() const noexcept;
        [[nodiscard]] vk::Sampler                  getVkSampler() const noexcept;
        [[nodiscard]] vk::ImageLayout              getImageLayout() const noexcept;
        [[nodiscard]] virtual vulkan_type          getVkHandle() const noexcept override;

      private:
        const ImageAttachment* const mImageAttachment;
        vk::Device                   mDevice;
        uint32_t                     mQueueIndexOwner;
        vk::ImageCreateInfo          mImageCreateInfo;
        vk::ImageViewCreateInfo      mImageViewCreateInfo;
        Texture                      mTexture;
        VulkanMemoryAllocator*       mAllocator;
    };

} // namespace nuts
