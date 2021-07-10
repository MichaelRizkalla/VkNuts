// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/Shader.h>

namespace nuts {
    AttachmentShaderFileAPI::AttachmentShaderFileAPI() = default;
    AttachmentShaderFileAPI::~AttachmentShaderFileAPI() { onUnload(); }
    void AttachmentShaderFileAPI::onLoad() noexcept {
        try {
            mShader = std::move(File::read< Container< uint32_t > >(getAttachmentName(), std::ios::binary));
        } catch (...) { return; }
    }
    void AttachmentShaderFileAPI::onUnload() noexcept { destroyVkShader(); }
    bool AttachmentShaderFileAPI::createVkShader(const vk::Device& device) noexcept {
        if (!mVkShader) {
            mDevice = device;
            // TODO: Better manage exceptions
            vk::ShaderModuleCreateInfo shaderModuleCreateInfo {};
            shaderModuleCreateInfo.setCodeSize(mShader.sizeInBytes());
            shaderModuleCreateInfo.setPCode(mShader.data());

            if (vk::Result::eSuccess != device.createShaderModule(&shaderModuleCreateInfo, nullptr, &mVkShader)) return false;
            return true;
        }
        return false;
    }
    void AttachmentShaderFileAPI::destroyVkShader() noexcept {
        if (mVkShader) mDevice.destroyShaderModule(mVkShader);
    }

    ShaderAttachment::ShaderAttachment(const char* name) : Attachment { name } {};
    ShaderAttachment::~ShaderAttachment() = default;
    ShaderAttachment::attachment_data ShaderAttachment::getAttachmentData() const { return attachment_data { mShader.data(), mShader.sizeInBytes(), mVkShader }; }

    ShaderRegistry::ShaderRegistry()  = default;
    ShaderRegistry::~ShaderRegistry() = default;
    void ShaderRegistry::init(RegistryInitializer* init) noexcept { mDevice = static_cast< ShaderRegistryInitializer* >(init)->mDevice; }
    bool ShaderRegistry::createVkShader(const char* alias) noexcept {
        if (!hasAttachment(alias)) { return false; }
        std::lock_guard guard { mMutex };
        return mRegistry.at(alias)->createVkShader(mDevice);
    }
    bool ShaderRegistry::destroyVkShader(const char* alias) noexcept {
        if (!hasAttachment(alias)) { return false; }
        std::lock_guard guard { mMutex };
        mRegistry.at(alias)->destroyVkShader();
        return true;
    }
    ShaderRegistry::attachment_data_type ShaderRegistry::queryAttachment(const char* alias) const {
        if (!mRegistry.contains(alias)) {
            NUTS_ENGINE_WARN("An attachment with name {} is not loaded!", alias);
            return attachment_data_type {};
        }
        // GetAttachmentData might throw
        auto mData = mRegistry.at(alias)->getAttachmentData();
        return mData;
    }

} // namespace nuts