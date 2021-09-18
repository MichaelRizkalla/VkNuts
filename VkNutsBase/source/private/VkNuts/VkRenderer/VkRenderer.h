#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/Module/Shader/ShaderRegistry.h>
#include <VkNuts/VkRenderer/Module/Image/ImageRegistry.h>
#include <VkNuts/VkRenderer/Module/Texture/TextureRegistry.h>
#include <Platform/Vulkan/VulkanHelper.h>
#include <Platform/Vulkan/VkManagement/VulkanMemoryAllocator.h>
#include <Platform/Vulkan/VkManagement/VkSamplerManager.h>
#include <Platform/Vulkan/VkManagement/SwapChain.h>
#include <Platform/Vulkan/VkManagement/CommandBuffer.h>
#include <Platform/Vulkan/VkManagement/FrameBuffer.h>

namespace nuts {
    class VkRenderer {
      public:
        STATIC_CLASS(VkRenderer)

        static void init(vk::Instance instance, vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surfaceKHR,
                         const QueueFamilyIndices& queueFamilyIndices, IWindow* window) noexcept;
        static void finalize() noexcept;

        [[nodiscard]] static bool             loadShader(const char* shaderPath, const char* alias) noexcept;
        [[nodiscard]] static bool             createShader(const char* alias) noexcept;
        [[nodiscard]] static bool             destroyShader(const char* alias) noexcept;
        [[nodiscard]] static vk::ShaderModule getVkShader(const char* alias) noexcept;

        [[nodiscard]] static bool loadImage(const char* imagePath, const char* alias, ImageLoadFormat fmt) noexcept;
        [[nodiscard]] static bool unloadImage(const char* alias) noexcept;

        [[nodiscard]] static bool createImage(const char* alias, const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        [[nodiscard]] static bool createTexture(const char* alias, const vk::ImageCreateInfo& imageCreateInfo, const vk::ImageViewCreateInfo& imageViewCreateInfo,
                                                const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept;
        [[nodiscard]] static bool destroyTexture(const char* alias) noexcept;

        [[nodiscard]] static vk::Image       getVkImage(const char* alias) noexcept;
        [[nodiscard]] static vk::ImageView   getVkImageView(const char* alias) noexcept;
        [[nodiscard]] static vk::ImageLayout getImageLayout(const char* alias) noexcept;
        [[nodiscard]] static vk::Sampler     getSampler(const char* alias) noexcept;

        [[nodiscard]] static vk::Format      getSwapChainImageFormat() noexcept;
        [[nodiscard]] static std::uint32_t   getSwapChainImageCount() noexcept;
        [[nodiscard]] static SwapChain::Data getSwapChainData() noexcept;
        static void                          createFrameBuffer(vk::RenderPass renderPass);
        static vk::CommandPool               createGraphicsCommandPool(const QueueFamilyIndices& queueIndices);
        static vk::CommandPool               createComputeCommandPool(const QueueFamilyIndices& queueIndices);
        static void                          createGraphicsCommandBuffers();

        [[nodiscard]] static uint32_t getMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

        [[nodiscard]] static vk::CommandPool getGraphicsCommandPool() noexcept;
        [[nodiscard]] static vk::CommandPool getComputeCommandPool() noexcept;
        [[nodiscard]] static vk::Queue       getGraphicsQueue() noexcept;
        [[nodiscard]] static vk::Queue       getComputeQueue() noexcept;
        [[nodiscard]] static vk::Queue       getPresentationQueue() noexcept;

        [[nodiscard]] static CommandBuffer&  getActiveGraphicsCommandBuffer(std::uint32_t currentFrame) noexcept;
        [[nodiscard]] static vk::Framebuffer getActiveFrameBuffer() noexcept;

        [[nodiscard]] static void deviceWaitIdle() noexcept;

        [[nodiscard]] static void updateRenderTarget();

        [[nodiscard]] static void prepareNextFrame() noexcept;
        [[nodiscard]] static void beginGraphicsCommandBuffer(std::uint32_t currentFrame);
        [[nodiscard]] static void endGraphicsCommandBuffer(std::uint32_t currentFrame);
        [[nodiscard]] static void beginGraphicsRenderPass(std::uint32_t currentFrame, const vk::RenderPassBeginInfo& renderPassBeginInfo);
        [[nodiscard]] static void endGraphicsRenderPass(std::uint32_t currentFrame);
        [[nodiscard]] static void submitGraphicsCommandBuffer(std::uint32_t currentFrame, Vector< vk::PipelineStageFlags > pipelineStageFlags);
        [[nodiscard]] static void present();

      private:
        static void setQueues(const QueueFamilyIndices& queueFamilyIndices) noexcept;

        template < class TRegistry, class Type = typename TRegistry::attachment_type::vulkan_type >
        [[nodiscard]] inline static Type queryVulkanRegistry(const TRegistry& registry, const char* alias) noexcept;

        [[nodiscard]] inline static IWindow*&                window() noexcept;
        [[nodiscard]] inline static vk::SurfaceKHR&          surfaceKHR() noexcept;
        [[nodiscard]] inline static vk::Instance&            instance() noexcept;
        [[nodiscard]] inline static vk::PhysicalDevice&      physicalDevice() noexcept;
        [[nodiscard]] inline static vk::Device&              device() noexcept;
        [[nodiscard]] inline static QueueFamilyIndices&      queueFamilyIndices() noexcept;
        [[nodiscard]] inline static vk::Queue&               graphicsQueue() noexcept;
        [[nodiscard]] inline static vk::Queue&               computeQueue() noexcept;
        [[nodiscard]] inline static vk::Queue&               presentationQueue() noexcept;
        [[nodiscard]] inline static vk::CommandPool&         graphicsCommandPool() noexcept;
        [[nodiscard]] inline static vk::CommandPool&         computeCommandPool() noexcept;
        [[nodiscard]] inline static ShaderRegistry&          shaderRegistry() noexcept;
        [[nodiscard]] inline static ImageRegistry&           imageRegistry() noexcept;
        [[nodiscard]] inline static TextureRegistry&         textureRegistry() noexcept;
        [[nodiscard]] inline static VulkanMemoryAllocator&   allocator() noexcept;
        [[nodiscard]] inline static VkSamplerManager&        samplerManager() noexcept;
        [[nodiscard]] inline static Texture&                 depthTexture(int width, int height);
        [[nodiscard]] inline static SwapChain&               swapChain() noexcept;
        [[nodiscard]] inline static Vector< CommandBuffer >& graphicsCommandBuffers() noexcept;
        [[nodiscard]] inline static FrameBuffer&             frameBuffer() noexcept;
    };

} // namespace nuts
