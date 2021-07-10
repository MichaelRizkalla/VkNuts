// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VulkanHelper.h>
#include <Platform/Vulkan/VulkanImage.h>

namespace nuts {

    vk::ImageMemoryBarrier VulkanImage::getImageMemoryBarrier(vk::Image image, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess, vk::ImageLayout oldLayout,
                                                              vk::ImageLayout newLayout /*, VK_IMAGE_ASPECT_COLOR_BIT*/) {
        return getImageMemoryBarrier(image, srcAccess, dstAccess, oldLayout, newLayout, vk::ImageAspectFlagBits::eColor);
    }
    vk::ImageMemoryBarrier VulkanImage::getImageMemoryBarrier(vk::Image image, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess, vk::ImageLayout oldLayout,
                                                              vk::ImageLayout newLayout, vk::ImageAspectFlags aspectFlags) {
        vk::ImageSubresourceRange subResourceRange {};
        subResourceRange.aspectMask = aspectFlags;
        subResourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
        subResourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

        return getImageMemoryBarrier(image, srcAccess, dstAccess, oldLayout, newLayout, subResourceRange);
    }
    vk::ImageMemoryBarrier VulkanImage::getImageMemoryBarrier(vk::Image image, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess, vk::ImageLayout oldLayout,
                                                              vk::ImageLayout newLayout, const vk::ImageSubresourceRange& subResourceRange) {
        vk::ImageMemoryBarrier barrier {};
        barrier.srcAccessMask    = srcAccess;
        barrier.dstAccessMask    = dstAccess;
        barrier.oldLayout        = oldLayout;
        barrier.newLayout        = newLayout;
        barrier.image            = image;
        barrier.subresourceRange = subResourceRange;

        return barrier;
    }
    uint32_t VulkanImage::getMaxMipLevels(VkExtent2D extent) { return static_cast< uint32_t >(std::floor(std::log2(std::max(extent.width, extent.height)))) + 1; }
    uint32_t VulkanImage::getMaxMipLevels(VkExtent3D extent) { return static_cast< uint32_t >(std::floor(std::log2(std::max(extent.width, extent.height)))) + 1; }
    constexpr vk::AccessFlags VulkanImage::getAccessFlags(vk::ImageLayout imageLayout) {
        switch (imageLayout) {

            case vk::ImageLayout::eShaderReadOnlyOptimal:
                return vk::AccessFlagBits::eShaderRead;

            case vk::ImageLayout::eColorAttachmentOptimal:
                return vk::AccessFlagBits::eColorAttachmentWrite;
            case vk::ImageLayout::eDepthStencilAttachmentOptimal:
                return vk::AccessFlagBits::eDepthStencilAttachmentWrite;

            case vk::ImageLayout::eTransferSrcOptimal:
                return vk::AccessFlagBits::eTransferRead;
            case vk::ImageLayout::eTransferDstOptimal:
                return vk::AccessFlagBits::eTransferWrite;

            case vk::ImageLayout::ePreinitialized:
                return vk::AccessFlagBits::eHostWrite;

            default:
                return vk::AccessFlagBits { 0 };
        }
    }
    constexpr vk::PipelineStageFlags VulkanImage::getPipelineStageFlags(vk::ImageLayout imageLayout) {
        switch (imageLayout) {

            case vk::ImageLayout::eShaderReadOnlyOptimal:
                return vk::PipelineStageFlagBits::eFragmentShader;

            case vk::ImageLayout::eTransferSrcOptimal:
            case vk::ImageLayout::eTransferDstOptimal:
                return vk::PipelineStageFlagBits::eTransfer;

            case vk::ImageLayout::eColorAttachmentOptimal:
                return vk::PipelineStageFlagBits::eColorAttachmentOutput;

            case vk::ImageLayout::eDepthStencilAttachmentOptimal:
                return vk::PipelineStageFlagBits::eEarlyFragmentTests;

            case vk::ImageLayout::ePreinitialized:
                return vk::PipelineStageFlagBits::eHost;

            case vk::ImageLayout::eUndefined:
                return vk::PipelineStageFlagBits::eTopOfPipe;

            default:
                return vk::PipelineStageFlagBits::eBottomOfPipe;
        }
    }
    void VulkanImage::changeImageLayout(vk::CommandBuffer commandBuffer, vk::Image image, vk::ImageLayout oldLayout,
                                        vk::ImageLayout newLayout /*, VK_IMAGE_ASPECT_COLOR_BIT*/) {
        changeImageLayout(commandBuffer, image, oldLayout, newLayout, vk::ImageAspectFlagBits::eColor);
    }
    void VulkanImage::changeImageLayout(vk::CommandBuffer commandBuffer, vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                                        vk::ImageAspectFlags aspectFlags) {
        vk::ImageSubresourceRange subResourceRange {};
        subResourceRange.aspectMask = aspectFlags;
        subResourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
        subResourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

        changeImageLayout(commandBuffer, image, oldLayout, newLayout, subResourceRange);
    }
    void VulkanImage::changeImageLayout(vk::CommandBuffer commandBuffer, vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                                        const vk::ImageSubresourceRange& subResourceRange) {
        auto imageMemoryBarrier = getImageMemoryBarrier(image, getAccessFlags(oldLayout), getAccessFlags(newLayout), oldLayout, newLayout, subResourceRange);
        auto srcStageMask       = getPipelineStageFlags(oldLayout);
        auto dstStageMask       = getPipelineStageFlags(newLayout);

        commandBuffer.pipelineBarrier(srcStageMask, dstStageMask, vk::DependencyFlagBits { 0 }, uint32_t { 0 }, nullptr, uint32_t { 0 }, nullptr, uint32_t { 0 },
                                      &imageMemoryBarrier);
    }

} // namespace nuts