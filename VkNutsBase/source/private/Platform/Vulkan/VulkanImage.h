#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

namespace nuts {

    struct VulkanImage {
        STATIC_CLASS(VulkanImage)

        NUTS_API [[nodiscard]] inline static vk::ImageMemoryBarrier           getImageMemoryBarrier(vk::Image image, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess,
                                                                                                    vk::ImageLayout oldLayout,
                                                                                                    vk::ImageLayout newLayout /*, VK_IMAGE_ASPECT_COLOR_BIT*/);
        NUTS_API [[nodiscard]] inline static vk::ImageMemoryBarrier           getImageMemoryBarrier(vk::Image image, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess,
                                                                                                    vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                                                                                                    vk::ImageAspectFlags aspectFlags);
        NUTS_API [[nodiscard]] inline static vk::ImageMemoryBarrier           getImageMemoryBarrier(vk::Image image, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess,
                                                                                                    vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                                                                                                    const vk::ImageSubresourceRange& subResourceRange);
        NUTS_API [[nodiscard]] inline static uint32_t                         getMaxMipLevels(VkExtent2D extent);
        NUTS_API [[nodiscard]] inline static uint32_t                         getMaxMipLevels(VkExtent3D extent);
        NUTS_API [[nodiscard]] inline constexpr static vk::AccessFlags        getAccessFlags(vk::ImageLayout imageLayout);
        NUTS_API [[nodiscard]] inline constexpr static vk::PipelineStageFlags getPipelineStageFlags(vk::ImageLayout imageLayout);
        NUTS_API static void changeImageLayout(vk::CommandBuffer commandBuffer, vk::Image image, vk::ImageLayout oldLayout,
                                               vk::ImageLayout newLayout /*, VK_IMAGE_ASPECT_COLOR_BIT*/);
        NUTS_API static void changeImageLayout(vk::CommandBuffer commandBuffer, vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                                               vk::ImageAspectFlags aspectFlags);
        NUTS_API static void changeImageLayout(vk::CommandBuffer commandBuffer, vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                                               const vk::ImageSubresourceRange& subResourceRange);

        // TODO: Should there be functions to generate vk::ImageCreateInfo or not?
    };
} // namespace nuts
