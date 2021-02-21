#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Generic/FileOperations.h>
#include <Platform/Vulkan/VulkanHelper.h>
#include <Utilities/Container.hpp>
#include <VkNuts/Core/Attachment/Attachment.h>
#include <VkNuts/Core/Registry/Registry.h>

namespace nuts {
    struct AttachmentShaderFileAPI : public AttachmentAPI {
        AttachmentShaderFileAPI();
        virtual ~AttachmentShaderFileAPI();
        void OnLoad() noexcept override;
        void OnUnload() noexcept override;
        bool CreateVkShader(const vk::Device& device) noexcept;
        void DestroyVkShader() noexcept;

      protected:
        Container< uint32_t > mShader;
        vk::Device            mDevice;
        vk::ShaderModule      mVkShader;
    };

    struct ShaderAttachmentData {
        const uint32_t* const pShaderCode;
        std::size_t           mShaderCodeSizeInBytes;
        vk::ShaderModule      mVkShaderModule;
    };

    struct ShaderAttachment final : public Attachment< ShaderAttachmentData, AttachmentShaderFileAPI > {
        ShaderAttachment(const char* name);
        ~ShaderAttachment();

        attachment_data GetAttachmentData() const override;
    };

    struct ShaderRegistryInitializer : RegistryInitializer {
        vk::Device mDevice;
    };

    class ShaderRegistry final : public Registry< ShaderAttachment > {
      public:
        ShaderRegistry();
        ~ShaderRegistry();

        void Init(RegistryInitializer* init) noexcept override;
        bool CreateVkShader(const char* alias) noexcept;
        bool DestroyVkShader(const char* alias) noexcept;

        attachment_type::attachment_data QueryAttachment(const char* alias) const override;

      private:
        vk::Device mDevice;
    };

} // namespace nuts