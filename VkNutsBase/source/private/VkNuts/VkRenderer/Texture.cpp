// clang-format off
#include <NutsPCH.h>
// clang-format on

#define STB_IMAGE_IMPLEMENTATION
#include <VkNuts/VkRenderer/Texture.h>

#include <VkNuts/VkRenderer/VkRenderer.h>

namespace nuts {

    AttachmentImageAPI::AttachmentImageAPI() = default;
    AttachmentImageAPI::~AttachmentImageAPI() { OnUnload(); }
    void AttachmentImageAPI::OnLoad() noexcept {
        mImageData = std::move(File::ReadImage< Container< uint8_t > >(GetAttachmentName(), mImageLoadFormat, &mImageWidth, &mImageHeight, &mImageChannels));
    }
    void AttachmentImageAPI::OnUnload() noexcept { DestroyVkImage(); }
    bool AttachmentImageAPI::CreateVkImage(const vk::Device& device, uint32_t queueIndexFamily, const vk::ImageCreateInfo* imageCreateInfo,
                                           const vk::ImageViewCreateInfo* imageViewCreateInfo) noexcept {
        mDevice              = device;
        mQueueIndexOwner     = queueIndexFamily;
        mImageCreateInfo     = *imageCreateInfo;
        mImageViewCreateInfo = *imageViewCreateInfo;

        if (vk::Result::eSuccess != device.createImage(&mImageCreateInfo, nullptr, &mVkImage)) return false;

        vk::MemoryRequirements memRequirements;
        device.getImageMemoryRequirements(mVkImage, &memRequirements);

        vk::DeviceMemory       mem;
        vk::MemoryAllocateInfo memAllocInfo {};
        memAllocInfo.allocationSize  = memRequirements.size;
        memAllocInfo.memoryTypeIndex = 10;
        // VkRenderer::GetMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible);

        if (vk::Result::eSuccess != device.allocateMemory(&memAllocInfo, nullptr, &mem)) { return false; }

        auto memPtr = device.mapMemory(mem, 0, VK_WHOLE_SIZE);
        std::memcpy(memPtr, mImageData.data(), mImageData.sizeInBytes());
        device.unmapMemory(mem);

        device.bindImageMemory(mVkImage, mem, 0);

        mImageViewCreateInfo.image = mVkImage;
        if (vk::Result::eSuccess != device.createImageView(&mImageViewCreateInfo, nullptr, &mVkImageView)) {
            device.destroyImage(mVkImage);
            return false;
        }
        return true;
    }
    void AttachmentImageAPI::DestroyVkImage() noexcept {
        if (mVkImageView) { mDevice.destroyImageView(mVkImageView); }
        if (mVkImage) { mDevice.destroyImage(mVkImage); }
    }
    bool AttachmentImageAPI::ChangeImageLayout(const vk::CommandBuffer& commandBuffer, vk::ImageLayout newImageLayout) noexcept {
        vk::ImageMemoryBarrier imageMemoryBarrier {};
        imageMemoryBarrier.oldLayout                       = mCurrentImageLayout;
        imageMemoryBarrier.newLayout                       = newImageLayout;
        imageMemoryBarrier.image                           = mVkImage;
        imageMemoryBarrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        imageMemoryBarrier.subresourceRange.baseMipLevel   = 0;
        imageMemoryBarrier.subresourceRange.levelCount     = 1;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount     = 1;
        imageMemoryBarrier.srcAccessMask                   = static_cast< vk::AccessFlagBits >(0);
        imageMemoryBarrier.dstAccessMask                   = static_cast< vk::AccessFlagBits >(0);

        commandBuffer.pipelineBarrier(static_cast< vk::PipelineStageFlagBits >(0), static_cast< vk::PipelineStageFlagBits >(0), static_cast< vk::DependencyFlagBits >(0),
                                      0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

        // TODO: Submit to a queue and finish transition
        return true;
    }

    ImageAttachment::ImageAttachment(const char* name, ImageLoadFormat fmt) : Attachment { name } { mImageLoadFormat = fmt; };
    ImageAttachment::~ImageAttachment() = default;
    ImageAttachment::attachment_data ImageAttachment::GetAttachmentData() const {
        return attachment_data { mImageData.data(), mImageWidth,  mImageHeight,     mImageChannels,       mImageData.sizeInBytes(),
                                mVkImage,          mVkImageView, mImageCreateInfo, mImageViewCreateInfo, mCurrentImageLayout };
    }

    ImageRegistry::ImageRegistry()  = default;
    ImageRegistry::~ImageRegistry() = default;

    void ImageRegistry::Init(RegistryInitializer* init) noexcept { mDevice = static_cast< ImageRegistryInitializer* >(init)->mDevice; }
    bool ImageRegistry::CreateVkImage(const char* alias, uint32_t queueIndexFamily, const vk::ImageCreateInfo* imageCreateInfo,
                                      const vk::ImageViewCreateInfo* imageViewCreateInfo) noexcept {
        if (!HasAttachment(alias)) { return false; }
        std::lock_guard guard { mMutex };
        return mRegistry.at(alias)->CreateVkImage(mDevice, queueIndexFamily, imageCreateInfo, imageViewCreateInfo);
    }
    bool ImageRegistry::DestroyVkImage(const char* alias) noexcept {
        if (!HasAttachment(alias)) { return false; }
        std::lock_guard guard { mMutex };
        mRegistry.at(alias)->DestroyVkImage();
        return true;
    }

    ImageRegistry::attachment_type::attachment_data ImageRegistry::QueryAttachment(const char* alias) const {
        if (!mRegistry.contains(alias)) {
            NUTS_ENGINE_WARN("An attachment with name {} is not loaded!", alias);
            return attachment_type::attachment_data {};
        }
        // GetAttachmentData might throw
        auto mData = mRegistry.at(alias)->GetAttachmentData();
        return mData;
    }

} // namespace nuts