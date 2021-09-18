// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/Module/Texture/Texture.h>
#include <Platform/Vulkan/VulkanImage.h>
#include <Platform/Generic/FileOperations.h>

namespace nuts {

    TextureAttachment::TextureAttachment(const char* name, VulkanMemoryAllocator* vulkanMemoryAllocator, const ImageAttachment* const imageAttachment) :
        VulkanAttachment { name },
        mImageAttachment { imageAttachment },
        mDevice {},
        mQueueIndexOwner { 0 },
        mImageCreateInfo {},
        mImageViewCreateInfo {},
        mAllocator { vulkanMemoryAllocator } {
    }
    TextureAttachment::~TextureAttachment() {
        if (mVkHandle.imageView || mVkHandle.image) {
            onUnload();
        }
    }

    bool TextureAttachment::onLoad() noexcept {
        if (mImageAttachment->data()) {
            return true;
        }
        return false;
    }
    bool TextureAttachment::onUnload() noexcept {
        destroyTexture();
        return true;
    }
    bool TextureAttachment::createImage(const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        mImageCreateInfo = imageCreateInfo;

        auto image            = mAllocator->createImage(mImageCreateInfo, memoryPropertyFlags);
        mVkHandle.image       = image.image;
        mVkHandle.allocation  = image.allocation;
        mVkHandle.imageLayout = image.imageLayout;

        if (!isVkImageCreated()) {
            return false;
        }
        return true;
    }
    bool TextureAttachment::createTexture(const vk::ImageViewCreateInfo& imageViewCreateInfo) noexcept {
        if (!isVkImageCreated()) {
            return false;
        }

        Image image {};
        image.image       = mVkHandle.image;
        image.allocation  = mVkHandle.allocation;
        image.imageLayout = mVkHandle.imageLayout;

        mVkHandle = mAllocator->createTexture(image, imageViewCreateInfo);

        if (!isVkTextureCreated()) {
            return false;
        }
        return true;
    }
    bool TextureAttachment::createTexture(const vk::ImageCreateInfo& imageCreateInfo, const vk::ImageViewCreateInfo& imageViewCreateInfo,
                                          const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        mImageCreateInfo     = imageCreateInfo;
        mImageViewCreateInfo = imageViewCreateInfo;

        mVkHandle = mAllocator->createTexture(mImageCreateInfo, memoryPropertyFlags, mImageViewCreateInfo);
        if (!isVkTextureCreated()) {
            return false;
        }
        return true;
    }
    void TextureAttachment::destroyTexture() noexcept {
        mAllocator->destroyTexture(mVkHandle);
    }
    void TextureAttachment::setSampler(vk::Sampler sampler) noexcept {
        mVkHandle.imageSampler = sampler;
    }
    bool TextureAttachment::changeImageLayout(const vk::CommandBuffer& commandBuffer, vk::ImageLayout newImageLayout) noexcept {
        VulkanImage::changeImageLayout(commandBuffer, mVkHandle.image, mVkHandle.imageLayout, newImageLayout);
        mVkHandle.imageLayout = newImageLayout;
        return true;
    }
    bool TextureAttachment::isVkImageCreated() const noexcept {
        return static_cast< bool >(mVkHandle.image);
    }
    bool TextureAttachment::isVkTextureCreated() const noexcept {
        return static_cast< bool >(mVkHandle.imageView);
    }
    TextureAttachment::attachment_data TextureAttachment::getData() const noexcept {
        return attachment_data { mImageAttachment, mImageCreateInfo, mImageViewCreateInfo, mVkHandle };
    }
    const ImageAttachment* const TextureAttachment::getImageAttachment() const noexcept {
        return mImageAttachment;
    }
    vk::Image TextureAttachment::getVkImage() const noexcept {
        return mVkHandle.image;
    }
    vk::Sampler TextureAttachment::getVkSampler() const noexcept {
        return mVkHandle.imageSampler;
    }
    vk::ImageLayout TextureAttachment::getImageLayout() const noexcept {
        return mVkHandle.imageLayout;
    }
    TextureAttachment::vulkan_type TextureAttachment::getVkHandle() const noexcept {
        return mVkHandle.imageView;
    }

} // namespace nuts
