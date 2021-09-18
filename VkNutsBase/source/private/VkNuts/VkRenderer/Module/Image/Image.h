#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Generic/ImageLoadFormat.h>
#include <Utilities/Container.hpp>
#include <VkNuts/Core/Attachment/Attachment.h>
#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>

namespace nuts {

    struct ImageAttachment final : public Attachment {

        struct ImageAttachmentData {
            const uint8_t* const pImageData;
            int                  mImageWidth;
            int                  mImageHeight;
            int                  mImageChannels;
            std::size_t          pImageDataSizeInBytes;
        };

        using attachment_data = ImageAttachmentData;

        [[nodiscard]] ImageAttachment(const char* name, ImageLoadFormat fmt);
        ~ImageAttachment();

        [[nodiscard]] bool                 onLoad() noexcept override;
        bool                               onUnload() noexcept override;
        [[nodiscard]] attachment_data      getData() const noexcept;
        [[nodiscard]] const uint8_t* const data() const noexcept;
        [[nodiscard]] std::size_t          size() const noexcept;

      private:
        Container< uint8_t > mImageData;
        ImageLoadFormat      mImageLoadFormat;
        int                  mImageWidth;
        int                  mImageHeight;
        int                  mImageChannels;
    };

} // namespace nuts
