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
    template < class VulkanType, class UnderlyingContainer = VulkanType >
    struct VulkanAttachment : public Attachment {

        using vulkan_type          = VulkanType;
        using underlying_container = UnderlyingContainer;

        [[nodiscard]] explicit VulkanAttachment(const char* name) : Attachment { name }, mVkHandle {} {
        }

        [[nodiscard]] virtual vulkan_type getVkHandle() const noexcept = 0;

      protected:
        underlying_container mVkHandle;
    };

} // namespace nuts
