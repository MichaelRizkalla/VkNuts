// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/Shader.h>

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
        return attachment_data { mShader.data(), mShader.sizeInBytes(), mDevice, mVkHandle };
    }

    ShaderRegistry::ShaderRegistry()  = default;
    ShaderRegistry::~ShaderRegistry() = default;
    void ShaderRegistry::init(const RegistryInitializer* const init) noexcept {
        // TODO: do a runtime type check with dynamic_cast
        mDefaultDevice = static_cast< const ShaderRegistryInitializer* const >(init)->mDefaultDevice;
    }
    bool ShaderRegistry::createVkShader(const char* alias) noexcept {
        if (!hasAttachment(alias)) {
            return false;
        }
        std::lock_guard guard { mMutex };
        return mRegistry.at(alias)->createVkShader(mDefaultDevice);
    }
    bool ShaderRegistry::destroyVkShader(const char* alias) noexcept {
        if (!hasAttachment(alias)) {
            return false;
        }
        std::lock_guard guard { mMutex };
        mRegistry.at(alias)->destroyVkShader();
        return true;
    }

} // namespace nuts