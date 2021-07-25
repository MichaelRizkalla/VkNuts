// clang-format off
#include <NutsPCH.h>
// clang-format on

#define STB_IMAGE_IMPLEMENTATION
#include <VkNuts/VkRenderer/Image.h>
#include <Platform/Vulkan/VulkanImage.h>

namespace nuts {

    ImageAttachment::ImageAttachment(const char* name, ImageLoadFormat fmt) : VulkanAttachment { name } {
        mImageLoadFormat = fmt;
    };
    ImageAttachment::~ImageAttachment() {
        if (mVkHandle) {
            onUnload();
        }
    }

    bool ImageAttachment::onLoad() noexcept {
        try {
            mImageData = std::move(File::readImage< Container< uint8_t > >(getAttachmentName(), mImageLoadFormat, &mImageWidth, &mImageHeight, &mImageChannels));
            return true;
        } catch (...) {
            return false;
        }
    }
    bool ImageAttachment::onUnload() noexcept {
        destroyVkImage();
        return true;
    }
    bool ImageAttachment::createVkImage(VulkanMemoryAllocator* alloc, const vk::ImageCreateInfo* imageCreateInfo,
                                        const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        mImageCreateInfo = *imageCreateInfo;
        mAllocator       = alloc;

        mImage = mAllocator->createImage(mImageCreateInfo, memoryPropertyFlags);
        if (!mImage.image) {
            return false;
        }
        return true;
    }
    void ImageAttachment::destroyVkImage() noexcept {
        mAllocator->destroyImage(mImage);
    }
    bool ImageAttachment::changeImageLayout(const vk::CommandBuffer& commandBuffer, vk::ImageLayout newImageLayout) noexcept {
        VulkanImage::changeImageLayout(commandBuffer, mImage.image, mImage.imageLayout, newImageLayout);
        mImage.imageLayout = newImageLayout;
        return true;
    }
    ImageAttachment::attachment_data ImageAttachment::getData() const {
        return attachment_data { mImageData.data(), mImageWidth, mImageHeight, mImageChannels, mImageData.sizeInBytes(), mImageCreateInfo, mImageViewCreateInfo, mImage };
    }

    ImageRegistry::ImageRegistry()  = default;
    ImageRegistry::~ImageRegistry() = default;
    void ImageRegistry::init(const RegistryInitializer* const init) noexcept {
        // TODO: do a runtime type check with dynamic_cast
        auto init_ = static_cast< const ImageRegistryInitializer* const >(init);

        mDefaultDevice = init_->mDefaultDevice;
        mAllocator = init_->mAllocator;
    }
    bool ImageRegistry::createVkImage(const char* alias, const vk::ImageCreateInfo* imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        if (!hasAttachment(alias)) {
            return false;
        }
        std::lock_guard guard { mMutex };
        return mRegistry.at(alias)->createVkImage(mAllocator, imageCreateInfo, memoryPropertyFlags);
    }
    bool ImageRegistry::destroyVkImage(const char* alias) noexcept {
        if (!hasAttachment(alias)) {
            return false;
        }
        std::lock_guard guard { mMutex };
        mRegistry.at(alias)->destroyVkImage();
        return true;
    }

} // namespace nuts