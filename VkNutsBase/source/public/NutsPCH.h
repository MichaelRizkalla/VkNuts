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

#include <VkNuts/Core/CoreDefines.h>
#if defined(NUTS_ENABLE_LOG)
    #include <VkNuts/Core/Log/Log.h>
#endif