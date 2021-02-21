#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/SingeltonCRTP.hpp>
#include <VkNuts/Core/Registry/Registry.h>
#include <VkNuts/Module/Module.h>

namespace nuts {
    class ModuleRegistry : Singelton< ModuleRegistry > {
      protected:
        ModuleRegistry()  = default;
        ~ModuleRegistry() = default;

      private:
        std::unordered_map< std::string, Module > mModules;
    };
} // namespace nuts