#pragma once

#include <Platform/CompilerDefines.h>
#include <Platform/PlatformDefines.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <memory_resource>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <Utilities/TypeAlias.h>
#include <VkNuts/Core/CoreDefines.h>
#if defined(NUTS_BUILD_ENGINE)
    #include <VkNuts/Core/Log/Log.h>

    #define VULKAN_HPP_STORAGE_SHARED
    #if defined(NUTS_BUILD_DLL)
        #define VULKAN_HPP_STORAGE_SHARED_EXPORT
    #endif

    #if defined(NUTS_OS_WINDOWS)
        #define VK_USE_PLATFORM_WIN32_KHR
    #else
        #error "OS support to be implemented!"
    #endif
    #define VK_NO_PROTOTYPES
    #include <vulkan/vulkan.hpp>
#endif
