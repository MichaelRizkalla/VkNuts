#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Generic/FileOperations.h>
#include <Utilities/Container.hpp>
#include <VkNuts/Core/Attachment/Attachment.h>
#include <VkNuts/Core/Registry/Registry.h>
#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>

namespace nuts {
    struct AttachmentTextureAPI : public AttachmentAPI {
        AttachmentTextureAPI();
        virtual ~AttachmentTextureAPI();
        void onLoad() noexcept override;
        void onUnload() noexcept override;
        bool createVkImage(VulkanMemoryAllocator* alloc, const vk::ImageCreateInfo* imageCreateInfo, const vk::ImageViewCreateInfo* imageViewCreateInfo,
                           const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        void destroyVkImage() noexcept;
        bool changeImageLayout(const vk::CommandBuffer& commandBuffer, vk::ImageLayout newImageLayout) noexcept;
        // bool ChangeImageQueueFamilyIndex(const vk::CommandBuffer& commandBuffer, uint32_t newQueueFamilyIndex) noexcept;

      protected:
        Container< uint8_t >    mImageData;
        ImageLoadFormat         mImageLoadFormat;
        int                     mImageWidth;
        int                     mImageHeight;
        int                     mImageChannels;
        vk::Device              mDevice;
        uint32_t                mQueueIndexOwner;
        vk::ImageCreateInfo     mImageCreateInfo;
        vk::ImageViewCreateInfo mImageViewCreateInfo;
        Texture                 mTexture;
        VulkanMemoryAllocator*  mAllocator;
    };

    struct ImageAttachmentData {
        const uint8_t* const    pImageData;
        int                     mImageWidth;
        int                     mImageHeight;
        int                     mImageChannels;
        std::size_t             pImageDataSizeInBytes;
        vk::ImageCreateInfo     mImageCreateInfo;
        vk::ImageViewCreateInfo mImageViewCreateInfo;
        Texture                 mTexture;
    };

    struct ImageAttachment final : public Attachment< ImageAttachmentData, AttachmentTextureAPI > {
        ImageAttachment(const char* name, ImageLoadFormat fmt);
        ~ImageAttachment();

        attachment_data getAttachmentData() const override;
    };

    struct ImageRegistryInitializer : RegistryInitializer {
        vk::Device             mDevice;
        VulkanMemoryAllocator* mAllocator;
    };

    class ImageRegistry final : public Registry< ImageAttachment > {
      public:
        ImageRegistry();
        ~ImageRegistry();

        void init(RegistryInitializer* init) noexcept override;
        bool createVkImage(const char* alias, const vk::ImageCreateInfo* imageCreateInfo, const vk::ImageViewCreateInfo* imageViewCreateInfo,
                           const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        bool destroyVkImage(const char* alias) noexcept;

        attachment_data_type queryAttachment(const char* alias) const override;

      private:
        vk::Device             mDevice;
        VulkanMemoryAllocator* mAllocator;
    };

} // namespace nuts