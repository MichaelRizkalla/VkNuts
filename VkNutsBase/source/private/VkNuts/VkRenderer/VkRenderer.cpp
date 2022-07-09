// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/VkRenderer.h>

namespace nuts {

    void VkRenderer::init(vk::Instance in_instance, vk::Device in_device, vk::PhysicalDevice in_physicalDevice, vk::SurfaceKHR in_surfaceKHR,
                          const QueueFamilyIndices& in_queueFamilyIndices, IWindow* in_window) noexcept {
        instance()           = in_instance;
        physicalDevice()     = in_physicalDevice;
        device()             = in_device;
        surfaceKHR()         = in_surfaceKHR;
        queueFamilyIndices() = in_queueFamilyIndices;
        window()             = in_window;

        setQueues(in_queueFamilyIndices);

        swapChain().init(in_device, in_physicalDevice, in_surfaceKHR, in_queueFamilyIndices, in_window);
        swapChain().createSwapChain();

        ShaderRegistryInitializer shaderRegistryInit {};
        shaderRegistryInit.mDefaultDevice = in_device;
        shaderRegistry().init(&shaderRegistryInit);

        ImageRegistryInitializer imageRegistryInit {};
        imageRegistry().init(&imageRegistryInit);

        allocator().init(in_instance, in_device, in_physicalDevice);

        NUTS_ENGINE_INFO("VkRenderer is initialized!");
    }
    void VkRenderer::finalize() noexcept {
        allocator().destroyTexture(depthTexture(0, 0));

        frameBuffer().finalize();

        for (auto& mGraphicsCommandBuffer : graphicsCommandBuffers()) {
            mGraphicsCommandBuffer.finalize();
        }

        if (getGraphicsCommandPool()) {
            device().destroyCommandPool(getGraphicsCommandPool());
        }
        if (getComputeCommandPool()) {
            device().destroyCommandPool(getComputeCommandPool());
        }

        shaderRegistry().detachAllAttachments();
        imageRegistry().detachAllAttachments();

        samplerManager().finalize();
        allocator().finalize();

        swapChain().finalize();

        instance().destroySurfaceKHR(surfaceKHR());
        device().destroy();
        instance().destroy();
    }

    bool VkRenderer::loadShader(const char* shaderPath, const char* alias) noexcept {
        return shaderRegistry().createAttachmentFromDesk(alias, shaderPath);
    }
    bool VkRenderer::createShader(const char* alias) noexcept {
        return shaderRegistry().createVkShader(alias);
    }
    bool VkRenderer::destroyShader(const char* alias) noexcept {
        return shaderRegistry().destroyVkShader(alias);
    }
    vk::ShaderModule VkRenderer::getVkShader(const char* alias) noexcept {
        return VkRenderer::queryVulkanRegistry(shaderRegistry(), alias);
        /* auto attachment = mShaderRegistry.getAttachment(alias);
        return attachment ? attachment->getVkHandle() : VK_NULL_HANDLE;*/
    }

    bool VkRenderer::loadImage(const char* imagePath, const char* alias, ImageLoadFormat fmt) noexcept {
        return imageRegistry().createAttachmentFromDesk(alias, imagePath, fmt);
    }
    bool VkRenderer::unloadImage(const char* alias) noexcept {
        return imageRegistry().detachAttachment(alias);
    }

    bool VkRenderer::createImage(const char* alias, const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        // TODO: fix
        return textureRegistry().createImage(alias, imageCreateInfo, memoryPropertyFlags);
    }
    bool VkRenderer::createTexture(const char* alias, const vk::ImageCreateInfo& imageCreateInfo, const vk::ImageViewCreateInfo& imageViewCreateInfo,
                                   const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        // TODO: fix
        return textureRegistry().createTexture(alias, imageCreateInfo, imageViewCreateInfo, memoryPropertyFlags);
    }
    bool VkRenderer::destroyTexture(const char* alias) noexcept {
        return textureRegistry().destroyTexture(alias);
    }

    vk::Image VkRenderer::getVkImage(const char* alias) noexcept {
        auto attachment = textureRegistry().getAttachment(alias);
        return attachment ? attachment->getVkImage() : VK_NULL_HANDLE;
    }
    vk::ImageView VkRenderer::getVkImageView(const char* alias) noexcept {
        auto attachment = textureRegistry().getAttachment(alias);
        return attachment ? attachment->getVkHandle() : VK_NULL_HANDLE;
    }
    vk::ImageLayout VkRenderer::getImageLayout(const char* alias) noexcept {
        auto attachment = textureRegistry().getAttachment(alias);
        return attachment ? attachment->getImageLayout() : vk::ImageLayout::eUndefined;
    }
    vk::Sampler VkRenderer::getSampler(const char* alias) noexcept {
        auto attachment = textureRegistry().getAttachment(alias);
        return attachment ? attachment->getVkSampler() : VK_NULL_HANDLE;
    }

    vk::Format VkRenderer::getSwapChainImageFormat() noexcept {
        return swapChain().getSwapChainImageFormat();
    }
    std::uint32_t VkRenderer::getSwapChainImageCount() noexcept {
        return static_cast< std::uint32_t >(swapChain().getSwapChainImageCount());
    }
    SwapChain::Data VkRenderer::getSwapChainData() noexcept {
        return swapChain().getData();
    }
    void VkRenderer::createFrameBuffer(vk::RenderPass renderPass) {
        frameBuffer().init(device(), renderPass);
        frameBuffer().createFrameBuffers(getSwapChainData());
    }
    vk::CommandPool VkRenderer::createGraphicsCommandPool(const QueueFamilyIndices& queueIndices) {
        vk::CommandPoolCreateInfo commandPoolCreateInfo {};
        commandPoolCreateInfo.flags            = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        commandPoolCreateInfo.queueFamilyIndex = queueIndices.graphicsFamily.value();

        if (vk::Result::eSuccess != device().createCommandPool(&commandPoolCreateInfo, nullptr, &graphicsCommandPool())) {
            // TODO: Better manage exceptions
            throw std::runtime_error("Failed to create Command Pool!");
        }
        return getGraphicsCommandPool();
    }
    vk::CommandPool VkRenderer::createComputeCommandPool(const QueueFamilyIndices& queueIndices) {
        vk::CommandPoolCreateInfo commandPoolCreateInfo {};
        commandPoolCreateInfo.flags            = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        commandPoolCreateInfo.queueFamilyIndex = queueIndices.computeFamily.value();

        if (vk::Result::eSuccess != device().createCommandPool(&commandPoolCreateInfo, nullptr, &computeCommandPool())) {
            // TODO: Better manage exceptions
            throw std::runtime_error("Failed to create Command Pool!");
        }
        return getComputeCommandPool();
    }
    void VkRenderer::createGraphicsCommandBuffers() {
        graphicsCommandBuffers().resize(getSwapChainImageCount());
        for (auto& mGraphicsCommandBuffer : graphicsCommandBuffers()) {
            mGraphicsCommandBuffer.init(device(), graphicsCommandPool());
            mGraphicsCommandBuffer.createCommandBuffer();
        }
    }

    uint32_t VkRenderer::getMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
        vk::PhysicalDeviceMemoryProperties memProperties;
        physicalDevice().getMemoryProperties(&memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        return 0;
    }

    vk::CommandPool VkRenderer::getGraphicsCommandPool() noexcept {
        return graphicsCommandPool();
    }
    vk::CommandPool VkRenderer::getComputeCommandPool() noexcept {
        return computeCommandPool();
    }
    vk::Queue VkRenderer::getGraphicsQueue() noexcept {
        return graphicsQueue();
    }
    vk::Queue VkRenderer::getComputeQueue() noexcept {
        return computeQueue();
    }
    vk::Queue VkRenderer::getPresentationQueue() noexcept {
        return presentationQueue();
    }

    CommandBuffer& VkRenderer::getActiveGraphicsCommandBuffer(std::uint32_t currentFrame) noexcept {
        return graphicsCommandBuffers().at(currentFrame % getSwapChainImageCount());
    }
    vk::Framebuffer VkRenderer::getActiveFrameBuffer() noexcept {
        return frameBuffer().getFrameBuffers()->at(swapChain().getActiveImageIndex());
    }

    void VkRenderer::deviceWaitIdle() noexcept {
        device().waitIdle();
    }

    void VkRenderer::updateRenderTarget() {
        swapChain().updateSwapChain(getPresentationQueue());
        frameBuffer().updateFrameBuffers(getSwapChainData());
    }

    void VkRenderer::prepareNextFrame() noexcept {
        swapChain().acquireNextImage();
    }
    void VkRenderer::beginGraphicsCommandBuffer(std::uint32_t currentFrame) {
        auto commandBuffer = graphicsCommandBuffers().at(currentFrame % getSwapChainImageCount());

        vk::CommandBufferBeginInfo commandBufferBeginInfo {};
        commandBufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
        if (vk::Result::eSuccess != commandBuffer.getVkHandle().begin(&commandBufferBeginInfo)) {
            // TODO: better manage exceptions
            throw std::exception("Failed to begin recording Command Buffer!");
        }
    }
    void VkRenderer::endGraphicsCommandBuffer(std::uint32_t currentFrame) {
        graphicsCommandBuffers().at(currentFrame % getSwapChainImageCount()).getVkHandle().end();
    }
    void VkRenderer::beginGraphicsRenderPass(std::uint32_t currentFrame, const vk::RenderPassBeginInfo& renderPassBeginInfo) {
        graphicsCommandBuffers().at(currentFrame % getSwapChainImageCount()).getVkHandle().beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
    }
    void VkRenderer::endGraphicsRenderPass(std::uint32_t currentFrame) {
        graphicsCommandBuffers().at(currentFrame % getSwapChainImageCount()).getVkHandle().endRenderPass();
    }
    void VkRenderer::submitGraphicsCommandBuffer(std::uint32_t currentFrame, Vector< vk::PipelineStageFlags > pipelineStageFlags) {
        swapChain().submit(graphicsQueue(), graphicsCommandBuffers().at(currentFrame % getSwapChainImageCount()).getVkHandle(), pipelineStageFlags);
    }
    void VkRenderer::present() {
        swapChain().present(presentationQueue());
    }

    void VkRenderer::setQueues(const QueueFamilyIndices& queueFamilyIndices) noexcept {
        auto& mDevice = device();

        graphicsQueue()     = VulkanHelper::getQueue(mDevice, queueFamilyIndices.graphicsFamily.value());
        computeQueue()      = VulkanHelper::getQueue(mDevice, queueFamilyIndices.computeFamily.value());
        presentationQueue() = VulkanHelper::getQueue(mDevice, queueFamilyIndices.presentFamily.value());
    }
    template < class TRegistry, class Type >
    inline Type VkRenderer::queryVulkanRegistry(const TRegistry& registry, const char* alias) noexcept {
        auto attachment = registry.getAttachment(alias);
        return attachment ? attachment->getVkHandle() : VK_NULL_HANDLE;
    }

    inline IWindow*& VkRenderer::window() noexcept {
        static IWindow* mWindow { nullptr };

        return mWindow;
    }
    inline vk::SurfaceKHR& VkRenderer::surfaceKHR() noexcept {
        static vk::SurfaceKHR mSurfaceKHR { nullptr };

        return mSurfaceKHR;
    }
    inline vk::Instance& VkRenderer::instance() noexcept {
        static vk::Instance mInstance { nullptr };

        return mInstance;
    }
    inline vk::PhysicalDevice& VkRenderer::physicalDevice() noexcept {
        static vk::PhysicalDevice mPhysicalDevice { nullptr };

        return mPhysicalDevice;
    }
    inline vk::Device& VkRenderer::device() noexcept {
        static vk::Device mDevice { nullptr };

        return mDevice;
    }
    inline QueueFamilyIndices& VkRenderer::queueFamilyIndices() noexcept {
        static QueueFamilyIndices mQueueFamilyIndices {};

        return mQueueFamilyIndices;
    }
    inline vk::Queue& VkRenderer::graphicsQueue() noexcept {
        static vk::Queue mQueue { nullptr };

        return mQueue;
    }
    inline vk::Queue& VkRenderer::computeQueue() noexcept {
        static vk::Queue mQueue { nullptr };

        return mQueue;
    }
    inline vk::Queue& VkRenderer::presentationQueue() noexcept {
        static vk::Queue mQueue { nullptr };

        return mQueue;
    }
    inline vk::CommandPool& VkRenderer::graphicsCommandPool() noexcept {
        static vk::CommandPool mCommandPool { nullptr };

        return mCommandPool;
    }
    inline vk::CommandPool& VkRenderer::computeCommandPool() noexcept {
        static vk::CommandPool mCommandPool { nullptr };

        return mCommandPool;
    }
    inline ShaderRegistry& VkRenderer::shaderRegistry() noexcept {
        static ShaderRegistry mShaderRegistry {};

        return mShaderRegistry;
    }
    inline ImageRegistry& VkRenderer::imageRegistry() noexcept {
        static ImageRegistry mImageRegistry {};

        return mImageRegistry;
    }
    inline TextureRegistry& VkRenderer::textureRegistry() noexcept {
        static TextureRegistry mTextureRegistry {};

        return mTextureRegistry;
    }
    inline VulkanMemoryAllocator& VkRenderer::allocator() noexcept {
        return *VulkanMemoryAllocator::getInstance();
    }
    inline VkSamplerManager& VkRenderer::samplerManager() noexcept {
        static VkSamplerManager mSamplerManager {};

        return mSamplerManager;
    }
    inline Texture& VkRenderer::depthTexture(int width, int height) {
        static Texture mDepthTexture {};
        static int     mWidth  = 0;
        static int     mHeight = 0;

        if (width == 0 && height == 0) {
            return mDepthTexture;
        }

        if (width != mWidth || height != mHeight) {
            mWidth  = width;
            mHeight = height;

            allocator().destroyTexture(mDepthTexture);

            const Vector< vk::Format >       formats { vk::Format::eD32SfloatS8Uint, vk::Format::eD32Sfloat, vk::Format::eD24UnormS8Uint };
            constexpr vk::ImageTiling        tiling       = vk::ImageTiling::eOptimal;
            constexpr vk::FormatFeatureFlags format_flags = vk::FormatFeatureFlagBits::eDepthStencilAttachment;

            auto                format = VulkanHelper::selectImageFormat(physicalDevice(), formats, tiling, format_flags);
            vk::ImageCreateInfo imageCreateInfo {};
            imageCreateInfo.imageType     = vk::ImageType::e2D;
            imageCreateInfo.extent.width  = mWidth;
            imageCreateInfo.extent.height = mHeight;
            imageCreateInfo.extent.depth  = 1;
            imageCreateInfo.mipLevels     = 1;
            imageCreateInfo.arrayLayers   = 1;
            imageCreateInfo.format        = format;
            imageCreateInfo.tiling        = tiling;
            imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;
            imageCreateInfo.usage         = vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eInputAttachment;
            imageCreateInfo.samples       = vk::SampleCountFlagBits::e1;
            imageCreateInfo.sharingMode   = vk::SharingMode::eExclusive;

            auto image = allocator().createImage(imageCreateInfo, vk::MemoryPropertyFlagBits::eDeviceLocal);

            vk::ImageViewCreateInfo imageViewCreateInfo {};
            imageViewCreateInfo.image                           = image.image;
            imageViewCreateInfo.viewType                        = vk::ImageViewType::e2D;
            imageViewCreateInfo.format                          = format;
            imageViewCreateInfo.components.r                    = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.components.g                    = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.components.b                    = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.components.a                    = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
            imageViewCreateInfo.subresourceRange.baseMipLevel   = 0;
            imageViewCreateInfo.subresourceRange.levelCount     = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount     = 1;

            mDepthTexture = allocator().createTexture(image, imageViewCreateInfo);
        }

        return mDepthTexture;
    }
    inline SwapChain& VkRenderer::swapChain() noexcept {
        static SwapChain mSwapChain {};

        return mSwapChain;
    }
    inline Vector< CommandBuffer >& VkRenderer::graphicsCommandBuffers() noexcept {
        static Vector< CommandBuffer > mGraphicsCommandBuffers {};

        return mGraphicsCommandBuffers;
    }
    inline FrameBuffer& VkRenderer::frameBuffer() noexcept {
        static FrameBuffer mFrameBuffer {};

        return mFrameBuffer;
    }

} // namespace nuts
