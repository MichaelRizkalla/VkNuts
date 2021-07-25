#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Generic/FileOperations.h>
#include <Platform/Vulkan/VulkanHelper.h>
#include <Utilities/Container.hpp>
#include <VkNuts/Core/Attachment/VulkanAttachment.h>
#include <VkNuts/Core/Registry/Registry.h>

namespace nuts {

    struct ShaderAttachment final : public VulkanAttachment< vk::ShaderModule > {

        struct ShaderAttachmentData {
            const uint32_t* const pShaderCode;
            std::size_t           mShaderCodeSizeInBytes;
            vk::Device            mDevice;
            vk::ShaderModule      mVkShaderModule;
        };

        using attachment_data = ShaderAttachmentData;

        explicit ShaderAttachment(const char* name);
        ~ShaderAttachment();

        bool            onLoad() noexcept override;
        bool            onUnload() noexcept override;
        bool            createVkShader(const vk::Device& device) noexcept;
        void            destroyVkShader() noexcept;
        attachment_data getData() const noexcept;

      protected:
        Container< uint32_t > mShader;
        vk::Device            mDevice;
    };

    struct ShaderRegistryInitializer final : RegistryInitializer {
        vk::Device mDefaultDevice;
    };

    class ShaderRegistry final : public Registry< ShaderAttachment > {
      public:
        ShaderRegistry();
        ~ShaderRegistry();

        void init(const RegistryInitializer* const init) noexcept override;
        bool createVkShader(const char* alias) noexcept;
        bool destroyVkShader(const char* alias) noexcept;

      private:
        vk::Device mDefaultDevice;
    };

} // namespace nuts