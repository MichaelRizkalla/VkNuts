#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Registry/Registry.h>
#include <VkNuts/Core/Layer/Layer.h>

namespace nuts {

    struct LayerRegistryInitializer final : RegistryInitializer {
        /* RESERVED */
    };

    class LayerRegistry : public Registry< Layer > {
      public:
        void init(const RegistryInitializer* const init) noexcept override;
    };

} // namespace nuts
