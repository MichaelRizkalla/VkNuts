#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/SingletonCRTP.hpp>
#include <VkNuts/Core/Registry/Registry.h>
#include <VkNuts/Module/Module.h>

namespace nuts {
    class ModuleRegistry : Singleton< ModuleRegistry > {
      protected:
        ModuleRegistry();
        ~ModuleRegistry();

      private:
        std::unordered_map< std::string, Module > mModules;
    };
} // namespace nuts