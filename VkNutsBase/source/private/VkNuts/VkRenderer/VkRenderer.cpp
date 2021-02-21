// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/VkRenderer.h>

namespace nuts {
    ShaderRegistry     VkRenderer::mShaderRegistry;
    ImageRegistry      VkRenderer::mImageRegistry;
    vk::PhysicalDevice VkRenderer::mPhysicalDevice;

    void VkRenderer::Init(vk::Device device) noexcept {
        ShaderRegistryInitializer shaderRegistryInit {};
        shaderRegistryInit.mDevice = device;
        mShaderRegistry.Init(&shaderRegistryInit);

        ImageRegistryInitializer imageRegistryInit {};
        imageRegistryInit.mDevice = device;
        mImageRegistry.Init(&imageRegistryInit);

        NUTS_ENGINE_INFO("VkRenderer is initialized!");
    }
    void VkRenderer::Finalize() noexcept {
        mShaderRegistry.DetachAllAttachments();
        mImageRegistry.DetachAllAttachments();
    }

    bool             VkRenderer::LoadShader(const char* shaderPath, const char* alias) noexcept { return mShaderRegistry.AttachAttachment(alias, shaderPath); }
    bool             VkRenderer::CreateShader(const char* alias) noexcept { return mShaderRegistry.CreateVkShader(alias); }
    bool             VkRenderer::DestroyShader(const char* alias) noexcept { return mShaderRegistry.DestroyVkShader(alias); }
    vk::ShaderModule VkRenderer::GetShader(const char* alias) { return mShaderRegistry.QueryAttachment(alias).mVkShaderModule; }

    bool VkRenderer::Load_Image(const char* imagePath, const char* alias, ImageLoadFormat fmt) noexcept { return mImageRegistry.AttachAttachment(alias, imagePath, fmt); }
    bool VkRenderer::CreateImage(const char* alias, uint32_t queueIndexFamily, const vk::ImageCreateInfo* imageCreateInfo,
                                 const vk::ImageViewCreateInfo* imageViewCreateInfo) noexcept {
        return mImageRegistry.CreateVkImage(alias, queueIndexFamily, imageCreateInfo, imageViewCreateInfo);
    }
    bool          VkRenderer::DestroyImage(const char* alias) noexcept { return mImageRegistry.DestroyVkImage(alias); }
    vk::Image     VkRenderer::GetVkImage(const char* alias) { return mImageRegistry.QueryAttachment(alias).mVkImage; }
    vk::ImageView VkRenderer::GetVkImageView(const char* alias) { return mImageRegistry.QueryAttachment(alias).mVkImageView; }

    uint32_t VkRenderer::GetMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
        vk::PhysicalDeviceMemoryProperties memProperties;
        mPhysicalDevice.getMemoryProperties(&memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) { return i; }
        }

        return 0;
    }

} // namespace nuts