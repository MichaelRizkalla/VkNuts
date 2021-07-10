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
        void onLoad() noexcept override;
        void onUnload() noexcept override;
        bool createVkShader(const vk::Device& device) noexcept;
        void destroyVkShader() noexcept;

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

        attachment_data getAttachmentData() const override;
    };

    struct ShaderRegistryInitializer : RegistryInitializer {
        vk::Device mDevice;
    };

    class ShaderRegistry final : public Registry< ShaderAttachment > {
      public:
        ShaderRegistry();
        ~ShaderRegistry();

        void init(RegistryInitializer* init) noexcept override;
        bool createVkShader(const char* alias) noexcept;
        bool destroyVkShader(const char* alias) noexcept;

        attachment_data_type queryAttachment(const char* alias) const override;

      private:
        vk::Device mDevice;
    };

} // namespace nuts