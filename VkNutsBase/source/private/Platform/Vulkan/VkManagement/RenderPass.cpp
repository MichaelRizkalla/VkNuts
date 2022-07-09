#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VkManagement/RenderPass.h>

namespace nuts {

    void RenderPass::init(vk::Device device) noexcept {
        mDevice = device;
    }
    void RenderPass::finalize() noexcept {
        if (mRenderPass) {
            destroyRenderPass();
        }
    }
    void RenderPass::destroyRenderPass() noexcept {
        mDevice.destroyRenderPass(mRenderPass);
    }
    vk::RenderPass RenderPass::getVkHandle() const noexcept {
        return mRenderPass;
    }

    void RenderPass::createRenderPass(const Vector< vk::Format >& colorAttachmentFormats, vk::Format depthAttachmentFormat, uint32_t renderSubpassCount,
                                      vk::ImageLayout initialImageLayout, vk::ImageLayout finalImageLayout, bool doClearColor, bool doClearDepth) {
        Vector< vk::AttachmentDescription > attachmentDescriptions {};
        Vector< vk::AttachmentReference >   attachmentReferences {};

        const bool hasDepth = static_cast< bool >(depthAttachmentFormat != vk::Format::eUndefined);

        for (const auto colorAttachmentFormat : colorAttachmentFormats) {
            vk::AttachmentDescription attachmentDescription {};

            attachmentDescription.format         = colorAttachmentFormat;
            attachmentDescription.samples        = vk::SampleCountFlagBits::e1;
            attachmentDescription.loadOp         = doClearColor ?
                                                       vk::AttachmentLoadOp::eClear :
                                                       ((vk::ImageLayout::eUndefined == initialImageLayout) ? vk::AttachmentLoadOp::eDontCare : vk::AttachmentLoadOp::eLoad);
            attachmentDescription.storeOp        = vk::AttachmentStoreOp::eStore;
            attachmentDescription.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare;
            attachmentDescription.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
            attachmentDescription.initialLayout  = initialImageLayout;
            attachmentDescription.finalLayout    = finalImageLayout;

            vk::AttachmentReference attachmentReference {};
            attachmentReference.attachment = static_cast< uint32_t >(attachmentDescriptions.size());
            attachmentReference.layout     = vk::ImageLayout::eColorAttachmentOptimal;

            attachmentDescriptions.emplace_back(attachmentDescription);
            attachmentReferences.emplace_back(attachmentReference);
        }

        vk::AttachmentReference depthAttachmentReference {};
        if (hasDepth) {
            vk::AttachmentDescription depthAttachmentDescription {};
            depthAttachmentDescription.format         = depthAttachmentFormat;
            depthAttachmentDescription.samples        = vk::SampleCountFlagBits::e1;
            depthAttachmentDescription.loadOp         = doClearColor ? vk::AttachmentLoadOp::eClear : vk::AttachmentLoadOp::eLoad;
            depthAttachmentDescription.storeOp        = vk::AttachmentStoreOp::eStore;
            depthAttachmentDescription.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare;
            depthAttachmentDescription.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
            depthAttachmentDescription.initialLayout  = vk::ImageLayout::eDepthStencilAttachmentOptimal;
            depthAttachmentDescription.finalLayout    = vk::ImageLayout::eDepthStencilAttachmentOptimal;

            depthAttachmentReference.attachment = static_cast< uint32_t >(attachmentDescriptions.size());
            depthAttachmentReference.layout     = vk::ImageLayout::eDepthStencilAttachmentOptimal;

            attachmentDescriptions.push_back(depthAttachmentDescription);
        }

        Vector< vk::SubpassDescription > subpassDescriptions {};
        Vector< vk::SubpassDependency >  subpassDependencies {};

        for (uint32_t i = 0; i < renderSubpassCount; i++) {
            vk::SubpassDescription subpassDescription {};
            subpassDescription.pipelineBindPoint       = vk::PipelineBindPoint::eGraphics;
            subpassDescription.colorAttachmentCount    = static_cast< uint32_t >(attachmentReferences.size());
            subpassDescription.pColorAttachments       = attachmentReferences.data();
            subpassDescription.pDepthStencilAttachment = hasDepth ? &depthAttachmentReference : VK_NULL_HANDLE;

            vk::SubpassDependency subpassDependency {};
            subpassDependency.srcSubpass    = i == 0 ? (VK_SUBPASS_EXTERNAL) : (i - 1);
            subpassDependency.dstSubpass    = i;
            subpassDependency.srcStageMask  = vk::PipelineStageFlagBits::eColorAttachmentOutput;
            subpassDependency.dstStageMask  = vk::PipelineStageFlagBits::eColorAttachmentOutput;
            subpassDependency.srcAccessMask = static_cast< vk::AccessFlagBits >(0);
            subpassDependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

            subpassDescriptions.emplace_back(subpassDescription);
            subpassDependencies.emplace_back(subpassDependency);
        }

        vk::RenderPassCreateInfo renderPassCreateInfo {};
        renderPassCreateInfo.attachmentCount = static_cast< uint32_t >(attachmentDescriptions.size());
        renderPassCreateInfo.pAttachments    = attachmentDescriptions.data();
        renderPassCreateInfo.subpassCount    = static_cast< uint32_t >(subpassDescriptions.size());
        renderPassCreateInfo.pSubpasses      = subpassDescriptions.data();
        renderPassCreateInfo.dependencyCount = static_cast< uint32_t >(subpassDependencies.size());
        renderPassCreateInfo.pDependencies   = subpassDependencies.data();

        if (vk::Result::eSuccess != mDevice.createRenderPass(&renderPassCreateInfo, nullptr, &mRenderPass)) {
            throw std::runtime_error("Failed to create Render Pass!");
        }
    }

} // namespace nuts
