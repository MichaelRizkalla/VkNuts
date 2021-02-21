// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/Shader.h>

namespace nuts {
    AttachmentShaderFileAPI::AttachmentShaderFileAPI() = default;
    AttachmentShaderFileAPI ::~AttachmentShaderFileAPI() { OnUnload(); }
    void AttachmentShaderFileAPI::OnLoad() noexcept { mShader = std::move(File::Read< Container< uint32_t > >(GetAttachmentName(), std::ios::binary)); }
    void AttachmentShaderFileAPI::OnUnload() noexcept { DestroyVkShader(); }
    bool AttachmentShaderFileAPI::CreateVkShader(const vk::Device& device) noexcept {
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
    void AttachmentShaderFileAPI::DestroyVkShader() noexcept {
        if (mVkShader) mDevice.destroyShaderModule(mVkShader);
    }

    ShaderAttachment::ShaderAttachment(const char* name) : Attachment { name } {};
    ShaderAttachment ::~ShaderAttachment() = default;
    ShaderAttachment::attachment_data ShaderAttachment::GetAttachmentData() const { return attachment_data { mShader.data(), mShader.sizeInBytes(), mVkShader }; }

    ShaderRegistry::ShaderRegistry()  = default;
    ShaderRegistry::~ShaderRegistry() = default;
    void ShaderRegistry::Init(RegistryInitializer* init) noexcept { mDevice = static_cast< ShaderRegistryInitializer* >(init)->mDevice; }
    bool ShaderRegistry::CreateVkShader(const char* alias) noexcept {
        if (!HasAttachment(alias)) { return false; }
        std::lock_guard guard { mMutex };
        return mRegistry.at(alias)->CreateVkShader(mDevice);
    }
    bool ShaderRegistry::DestroyVkShader(const char* alias) noexcept {
        if (!HasAttachment(alias)) { return false; }
        std::lock_guard guard { mMutex };
        mRegistry.at(alias)->DestroyVkShader();
        return true;
    }
    ShaderRegistry::attachment_type::attachment_data ShaderRegistry::QueryAttachment(const char* alias) const {
        if (!mRegistry.contains(alias)) {
            NUTS_ENGINE_WARN("An attachment with name {} is not loaded!", alias);
            return attachment_type::attachment_data {};
        }
        // GetAttachmentData might throw
        auto mData = mRegistry.at(alias)->GetAttachmentData();
        return mData;
    }

} // namespace nuts