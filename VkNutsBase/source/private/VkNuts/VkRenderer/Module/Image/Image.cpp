// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/Module/Image/Image.h>
#include <Platform/Vulkan/VulkanImage.h>
#include <Platform/Generic/FileOperations.h>

namespace nuts {

    ImageAttachment::ImageAttachment(const char* name, ImageLoadFormat fmt) :
        Attachment { name }, mImageData {}, mImageWidth { 0 }, mImageHeight { 0 }, mImageChannels { 0 } {
        mImageLoadFormat = fmt;
    }
    ImageAttachment::~ImageAttachment() {
        if (mImageData.data()) {
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
        mImageData.reset();
        mImageWidth      = 0;
        mImageHeight     = 0;
        mImageChannels   = 0;
        mImageLoadFormat = ImageLoadFormat::Default;
        return true;
    }
    ImageAttachment::attachment_data ImageAttachment::getData() const noexcept {
        return attachment_data { mImageData.data(), mImageWidth, mImageHeight, mImageChannels, mImageData.sizeInBytes() };
    }
    const uint8_t* const ImageAttachment::data() const noexcept {
        return mImageData.data();
    }
    std::size_t ImageAttachment::size() const noexcept {
        return mImageData.size();
    }

} // namespace nuts
