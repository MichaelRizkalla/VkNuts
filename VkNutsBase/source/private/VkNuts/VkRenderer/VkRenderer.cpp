// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/VkRenderer.h>

namespace nuts {

    vk::Instance          VkRenderer::mInstance { nullptr };
    vk::PhysicalDevice    VkRenderer::mPhysicalDevice { nullptr };
    vk::Device            VkRenderer::mDevice { nullptr };
    ShaderRegistry        VkRenderer::mShaderRegistry {};
    ImageRegistry         VkRenderer::mImageRegistry {};
    VulkanMemoryAllocator VkRenderer::mAllocator {};
    VkSamplerManager      VkRenderer::mSamplerManager {};

    void VkRenderer::init(vk::Instance instance, vk::Device device, vk::PhysicalDevice physicalDevice) noexcept {
        mInstance       = instance;
        mPhysicalDevice = physicalDevice;
        mDevice         = device;

        ShaderRegistryInitializer shaderRegistryInit {};
        shaderRegistryInit.mDevice = mDevice;
        mShaderRegistry.init(&shaderRegistryInit);

        ImageRegistryInitializer imageRegistryInit {};
        imageRegistryInit.mDevice = mDevice;
        mImageRegistry.init(&imageRegistryInit);

        mAllocator.init(mInstance, mDevice, mPhysicalDevice);

        NUTS_ENGINE_INFO("VkRenderer is initialized!");
    }
    void VkRenderer::finalize() noexcept {
        mShaderRegistry.detachAllAttachments();
        mImageRegistry.detachAllAttachments();

        mSamplerManager.finalize();
        mAllocator.finalize();
    }

    bool             VkRenderer::loadShader(const char* shaderPath, const char* alias) noexcept { return mShaderRegistry.attachAttachment(alias, shaderPath); }
    bool             VkRenderer::createShader(const char* alias) noexcept { return mShaderRegistry.createVkShader(alias); }
    bool             VkRenderer::destroyShader(const char* alias) noexcept { return mShaderRegistry.destroyVkShader(alias); }
    vk::ShaderModule VkRenderer::getShader(const char* alias) { return mShaderRegistry.queryAttachment(alias).mVkShaderModule; }

    bool VkRenderer::loadImage(const char* imagePath, const char* alias, ImageLoadFormat fmt) noexcept { return mImageRegistry.attachAttachment(alias, imagePath, fmt); }
    bool VkRenderer::createImage(const char* alias, const vk::ImageCreateInfo* imageCreateInfo, const vk::ImageViewCreateInfo* imageViewCreateInfo,
                                 const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        return mImageRegistry.createVkImage(alias, imageCreateInfo, imageViewCreateInfo, memoryPropertyFlags);
    }
    bool          VkRenderer::destroyImage(const char* alias) noexcept { return mImageRegistry.destroyVkImage(alias); }
    vk::Image     VkRenderer::getVkImage(const char* alias) { return mImageRegistry.queryAttachment(alias).mTexture.image; }
    vk::ImageView VkRenderer::getVkImageView(const char* alias) { return mImageRegistry.queryAttachment(alias).mTexture.imageView; }

    uint32_t VkRenderer::getMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
        vk::PhysicalDeviceMemoryProperties memProperties;
        mPhysicalDevice.getMemoryProperties(&memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) { return i; }
        }

        return 0;
    }

} // namespace nuts