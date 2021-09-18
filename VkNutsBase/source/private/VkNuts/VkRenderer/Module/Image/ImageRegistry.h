#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Registry/Registry.h>
#include <VkNuts/VkRenderer/Module/Image/Image.h>

namespace nuts {

    struct ImageRegistryInitializer final : RegistryInitializer {
        /*Reserved*/
    };

    class ImageRegistry final : public Registry< ImageAttachment > {
      public:
        void init(const RegistryInitializer* const) noexcept override;
    };

} // namespace nuts
