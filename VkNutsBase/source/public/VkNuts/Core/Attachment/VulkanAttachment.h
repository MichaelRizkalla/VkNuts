#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Attachment/Attachment.h>

namespace nuts {

    /// <summary>
    ///
    /// </summary>
    /// <typeparam name="VulkanType"></typeparam>
    template < class VulkanType >
    struct VulkanAttachment : public Attachment {

        using vulkan_type = VulkanType;

        [[nodiscard]] explicit VulkanAttachment(const char* name) : Attachment { name }, mVkHandle {} {
        }

        [[nodiscard]] vulkan_type getVkHandle() const noexcept {
            return mVkHandle;
        }

      protected:
        vulkan_type mVkHandle;
    };

} // namespace nuts