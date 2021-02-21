#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Generic/FileOperations.h>
#include <Platform/Vulkan/VulkanHelper.h>
#include <Utilities/Container.hpp>
#include <VkNuts/Core/Attachment/Attachment.h>
#include <VkNuts/Core/Registry/Registry.h>

namespace nuts {
    struct AttachmentImageAPI : public AttachmentAPI {
        AttachmentImageAPI();
        virtual ~AttachmentImageAPI();
        void OnLoad() noexcept override;
        void OnUnload() noexcept override;
        bool CreateVkImage(const vk::Device& device, uint32_t queueIndexFamily, const vk::ImageCreateInfo* imageCreateInfo,
                           const vk::ImageViewCreateInfo* imageViewCreateInfo) noexcept;
        void DestroyVkImage() noexcept;
        bool ChangeImageLayout(const vk::CommandBuffer& commandBuffer, vk::ImageLayout newImageLayout) noexcept;
        // bool ChangeImageQueueFamilyIndex(const vk::CommandBuffer& commandBuffer, uint32_t newQueueFamilyIndex) noexcept;

      protected:
        Container< uint8_t >    mImageData;
        ImageLoadFormat         mImageLoadFormat;
        int                     mImageWidth;
        int                     mImageHeight;
        int                     mImageChannels;
        vk::Device              mDevice;
        uint32_t                mQueueIndexOwner;
        vk::Image               mVkImage;
        vk::ImageView           mVkImageView;
        vk::ImageCreateInfo     mImageCreateInfo;
        vk::ImageViewCreateInfo mImageViewCreateInfo;
        vk::ImageLayout         mCurrentImageLayout;
    };

    struct ImageAttachmentData {
        const uint8_t* const    pImageData;
        int                     mImageWidth;
        int                     mImageHeight;
        int                     mImageChannels;
        std::size_t             pImageDataSizeInBytes;
        vk::Image               mVkImage;
        vk::ImageView           mVkImageView;
        vk::ImageCreateInfo     mImageCreateInfo;
        vk::ImageViewCreateInfo mImageViewCreateInfo;
        vk::ImageLayout         mCurrentImageLayout;
    };

    struct ImageAttachment final : public Attachment< ImageAttachmentData, AttachmentImageAPI > {
        ImageAttachment(const char* name, ImageLoadFormat fmt);
        ~ImageAttachment();

        attachment_data GetAttachmentData() const override;
    };

    struct ImageRegistryInitializer : RegistryInitializer {
        vk::Device mDevice;
    };

    class ImageRegistry final : public Registry< ImageAttachment > {
      public:
        ImageRegistry();
        ~ImageRegistry();

        void Init(RegistryInitializer* init) noexcept override;
        bool CreateVkImage(const char* alias, uint32_t queueIndexFamily, const vk::ImageCreateInfo* imageCreateInfo,
                           const vk::ImageViewCreateInfo* imageViewCreateInfo) noexcept;
        bool DestroyVkImage(const char* alias) noexcept;

        attachment_type::attachment_data QueryAttachment(const char* alias) const override;

      private:
        vk::Device mDevice;
    };

} // namespace nuts