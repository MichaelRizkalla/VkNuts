#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

namespace nuts {

    class CommandBuffer {
      public:
        DEFAULT_CLASS(CommandBuffer)

        void init(vk::Device device, vk::CommandPool commandPool) noexcept;
        void finalize() noexcept;

        void createCommandBuffer();
        void destroyCommandBuffer();

        vk::CommandBuffer getVkHandle() const noexcept;

      private:
        vk::Device        mDevice;
        vk::CommandPool   mCommandPool;
        vk::CommandBuffer mCommandBuffer;
    };
} // namespace nuts
