#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Registry/Registry.h>
#include <VkNuts/VkRenderer/Module/Shader/Shader.h>

namespace nuts {

    struct ShaderRegistryInitializer final : RegistryInitializer {
        vk::Device mDefaultDevice;
    };

    class ShaderRegistry final : public Registry< ShaderAttachment > {
      public:
        void               init(const RegistryInitializer* const init) noexcept override;
        [[nodiscard]] bool createVkShader(const char* alias) noexcept;
        [[nodiscard]] bool destroyVkShader(const char* alias) noexcept;

      private:
        vk::Device mDefaultDevice;
    };

} // namespace nuts
