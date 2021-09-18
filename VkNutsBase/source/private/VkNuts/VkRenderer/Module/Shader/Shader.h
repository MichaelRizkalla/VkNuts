#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VulkanHelper.h>
#include <Utilities/Container.hpp>
#include <VkNuts/Core/Attachment/VulkanAttachment.h>
namespace nuts {

    struct ShaderAttachment final : public VulkanAttachment< vk::ShaderModule > {

        struct ShaderAttachmentData {
            vk::ShaderStageFlagBits shaderStageFlagBit;
            const uint32_t* const   pShaderCode;
            std::size_t             mShaderCodeSizeInBytes;
            vk::Device              mDevice;
            vk::ShaderModule        mVkShaderModule;
        };

        using attachment_data = ShaderAttachmentData;

        [[nodiscard]] explicit ShaderAttachment(const char* name);
        ~ShaderAttachment();

        [[nodiscard]] bool                onLoad() noexcept override;
        bool                              onUnload() noexcept override;
        [[nodiscard]] bool                createVkShader(const vk::Device& device) noexcept;
        void                              destroyVkShader() noexcept;
        [[nodiscard]] attachment_data     getData() const noexcept;
        [[nodiscard]] virtual vulkan_type getVkHandle() const noexcept override;

      protected:
        Container< uint32_t >   mShader;
        vk::ShaderStageFlagBits shaderStageFlagBit;
        vk::Device              mDevice;
    };

} // namespace nuts
