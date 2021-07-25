#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Generic/FileOperations.h>
#include <Utilities/Container.hpp>
#include <VkNuts/Core/Attachment/VulkanAttachment.h>
#include <VkNuts/Core/Registry/Registry.h>
#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>

namespace nuts {

    struct ImageAttachment final : public VulkanAttachment< vk::Image > {

        struct ImageAttachmentData {
            const uint8_t* const    pImageData;
            int                     mImageWidth;
            int                     mImageHeight;
            int                     mImageChannels;
            std::size_t             pImageDataSizeInBytes;
            vk::ImageCreateInfo     mImageCreateInfo;
            vk::ImageViewCreateInfo mImageViewCreateInfo;
            Image                   mImage;
        };

        using attachment_data = ImageAttachmentData;

        explicit ImageAttachment(const char* name, ImageLoadFormat fmt);
        ~ImageAttachment();

        bool onLoad() noexcept override;
        bool onUnload() noexcept override;
        bool createVkImage(VulkanMemoryAllocator* alloc, const vk::ImageCreateInfo* imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        void destroyVkImage() noexcept;
        bool changeImageLayout(const vk::CommandBuffer& commandBuffer, vk::ImageLayout newImageLayout) noexcept;
        // bool ChangeImageQueueFamilyIndex(const vk::CommandBuffer& commandBuffer, uint32_t newQueueFamilyIndex) noexcept;
        attachment_data getData() const;

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
        Image                   mImage;
        VulkanMemoryAllocator*  mAllocator;
    };

    struct ImageRegistryInitializer final : RegistryInitializer {
        vk::Device             mDefaultDevice;
        VulkanMemoryAllocator* mAllocator;
    };

    class ImageRegistry final : public Registry< ImageAttachment > {
      public:
        ImageRegistry();
        ~ImageRegistry();

        void init(const RegistryInitializer* const init) noexcept override;
        bool createVkImage(const char* alias, const vk::ImageCreateInfo* imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        bool destroyVkImage(const char* alias) noexcept;

      private:
        vk::Device             mDefaultDevice;
        VulkanMemoryAllocator* mAllocator;
    };

} // namespace nuts