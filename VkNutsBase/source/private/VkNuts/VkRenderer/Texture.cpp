// clang-format off
#include <NutsPCH.h>
// clang-format on

#define STB_IMAGE_IMPLEMENTATION
#include <VkNuts/VkRenderer/Texture.h>
#include <Platform/Vulkan/VulkanImage.h>

namespace nuts {
    AttachmentTextureAPI::AttachmentTextureAPI() = default;
    AttachmentTextureAPI::~AttachmentTextureAPI() { onUnload(); }
    void AttachmentTextureAPI::onLoad() noexcept {
        mImageData = std::move(File::readImage< Container< uint8_t > >(getAttachmentName(), mImageLoadFormat, &mImageWidth, &mImageHeight, &mImageChannels));
    }
    void AttachmentTextureAPI::onUnload() noexcept { destroyVkImage(); }
    bool AttachmentTextureAPI::createVkImage(VulkanMemoryAllocator* alloc, const vk::ImageCreateInfo* imageCreateInfo, const vk::ImageViewCreateInfo* imageViewCreateInfo,
                                             const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        mImageCreateInfo     = *imageCreateInfo;
        mImageViewCreateInfo = *imageViewCreateInfo;
        mAllocator           = alloc;

        mTexture = mAllocator->createTexture(mImageCreateInfo, memoryPropertyFlags, mImageViewCreateInfo);
        if (!mTexture.image) { return false; }
        return true;
    }
    void AttachmentTextureAPI::destroyVkImage() noexcept { mAllocator->destroyTexture(mTexture); }
    bool AttachmentTextureAPI::changeImageLayout(const vk::CommandBuffer& commandBuffer, vk::ImageLayout newImageLayout) noexcept {
        VulkanImage::changeImageLayout(commandBuffer, mTexture.image, mTexture.imageLayout, newImageLayout);
        mTexture.imageLayout = newImageLayout;
        return true;
    }

    ImageAttachment::ImageAttachment(const char* name, ImageLoadFormat fmt) : Attachment { name } { mImageLoadFormat = fmt; };
    ImageAttachment::~ImageAttachment() = default;
    ImageAttachment::attachment_data ImageAttachment::getAttachmentData() const {
        return attachment_data {
            mImageData.data(), mImageWidth, mImageHeight, mImageChannels, mImageData.sizeInBytes(), mImageCreateInfo, mImageViewCreateInfo, mTexture
        };
    }

    ImageRegistry::ImageRegistry()  = default;
    ImageRegistry::~ImageRegistry() = default;
    void ImageRegistry::init(RegistryInitializer* init) noexcept {
        mDevice    = static_cast< ImageRegistryInitializer* >(init)->mDevice;
        mAllocator = static_cast< ImageRegistryInitializer* >(init)->mAllocator;
    }
    bool ImageRegistry::createVkImage(const char* alias, const vk::ImageCreateInfo* imageCreateInfo, const vk::ImageViewCreateInfo* imageViewCreateInfo,
                                      const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        if (!hasAttachment(alias)) { return false; }
        std::lock_guard guard { mMutex };
        return mRegistry.at(alias)->createVkImage(mAllocator, imageCreateInfo, imageViewCreateInfo, memoryPropertyFlags);
    }
    bool ImageRegistry::destroyVkImage(const char* alias) noexcept {
        if (!hasAttachment(alias)) { return false; }
        std::lock_guard guard { mMutex };
        mRegistry.at(alias)->destroyVkImage();
        return true;
    }
    ImageRegistry::attachment_data_type ImageRegistry::queryAttachment(const char* alias) const {
        if (!mRegistry.contains(alias)) {
            NUTS_ENGINE_WARN("An attachment with name {} is not loaded!", alias);
            return attachment_data_type {};
        }
        // GetAttachmentData might throw
        auto mData = mRegistry.at(alias)->getAttachmentData();
        return mData;
    }
} // namespace nuts