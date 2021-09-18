// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/Module/Shader/Shader.h>
#include <Platform/Generic/FileOperations.h>

namespace nuts {

    ShaderAttachment::ShaderAttachment(const char* name) : VulkanAttachment { name } {};
    ShaderAttachment::~ShaderAttachment() {
        if (mVkHandle) {
            onUnload();
        }
    }
    bool ShaderAttachment::onLoad() noexcept {
        try {
            mShader = std::move(File::read< Container< uint32_t > >(getAttachmentName(), std::ios::binary));
            return true;
        } catch (...) {
            return false;
        }
    }
    bool ShaderAttachment::onUnload() noexcept {
        destroyVkShader();
        return true;
    }
    bool ShaderAttachment::createVkShader(const vk::Device& device) noexcept {
        if (!mVkHandle) {
            mDevice = device;
            // TODO: Better manage exceptions
            vk::ShaderModuleCreateInfo shaderModuleCreateInfo {};
            shaderModuleCreateInfo.setCodeSize(mShader.sizeInBytes());
            shaderModuleCreateInfo.setPCode(mShader.data());

            if (vk::Result::eSuccess != device.createShaderModule(&shaderModuleCreateInfo, nullptr, &mVkHandle))
                return false;
            return true;
        }
        return false;
    }
    void ShaderAttachment::destroyVkShader() noexcept {
        if (mVkHandle)
            mDevice.destroyShaderModule(mVkHandle);
    }
    ShaderAttachment::attachment_data ShaderAttachment::getData() const noexcept {
        return attachment_data { shaderStageFlagBit, mShader.data(), mShader.sizeInBytes(), mDevice, mVkHandle };
    }
    ShaderAttachment::vulkan_type ShaderAttachment::getVkHandle() const noexcept {
        return mVkHandle;
    }

} // namespace nuts
