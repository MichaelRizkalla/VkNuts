################################################################################
### Programming language version and compiler flags
################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(VkNutsBase CXX)

################################################################################
### Includes - source files, header files, moc files
################################################################################
set(EXTERN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../extern")

file(GLOB PLATFORM_PUBLIC "source/public/Platform/*.*")
file(GLOB PLATFORM_PRIVATE "source/private/Platform/*.*")
file(GLOB PLATFORM_GENERIC_PUBLIC "source/public/Platform/Generic/*.*")
file(GLOB PLATFORM_VULKAN_PUBLIC "source/public/Platform/Vulkan/*.*")
file(GLOB PLATFORM_VULKAN_PRIVATE "source/private/Platform/Vulkan/*.*")
file(GLOB PLATFORM_VULKAN_VKMANAGEMENT_PUBLIC "source/public/Platform/Vulkan/VkManagement/*.*")
file(GLOB PLATFORM_VULKAN_VKMANAGEMENT_PRIVATE "source/private/Platform/Vulkan/VkManagement/*.*")
file(GLOB VKNUTSBASE_PUBLIC "source/public/*.*")
file(GLOB VKNUTS_CORE_PUBLIC "source/public/VkNuts/Core/*.*")
file(GLOB VKNUTS_CORE_ATTACHMENT_PUBLIC "source/public/VkNuts/Core/Attachment/*.*")
file(GLOB VKNUTS_CORE_ENGINE_PRIVATE "source/private/VkNuts/Core/Engine/*.*")
file(GLOB VKNUTS_CORE_ENGINE_PUBLIC "source/public/VkNuts/Core/Engine/*.*")
file(GLOB VKNUTS_CORE_EVENT_PUBLIC "source/public/VkNuts/Core/Event/*.*")
file(GLOB VKNUTS_CORE_GUI_PRIVATE "source/private/VkNuts/Core/Gui/*.*")
file(GLOB VKNUTS_CORE_GUI_PUBLIC "source/public/VkNuts/Core/Gui/*.*")
file(GLOB VKNUTS_CORE_LAYER_PUBLIC "source/public/VkNuts/Core/Layer/*.*")
file(GLOB VKNUTS_CORE_LAYER_PRIVATE "source/private/VkNuts/Core/Layer/*.*")
file(GLOB VKNUTS_CORE_LOG_PUBLIC "source/public/VkNuts/Core/Log/*.*")
file(GLOB VKNUTS_CORE_LOG_PRIVATE "source/private/VkNuts/Core/Log/*.*")
file(GLOB VKNUTS_CORE_MEMORY_MANAGEMENT_PUBLIC "source/public/VkNuts/Core/MemoryManagement/*.*")
file(GLOB VKNUTS_CORE_MEMORY_MANAGEMENT_PRIVATE "source/private/VkNuts/Core/MemoryManagement/*.*")
file(GLOB VKNUTS_CORE_REGISTRY_PUBLIC "source/public/VkNuts/Core/Registry/*.*")
file(GLOB VKNUTS_CORE_WINDOW_PRIVATE "source/private/VkNuts/Core/Window/*.*")
file(GLOB VKNUTS_CORE_WINDOW_PUBLIC "source/public/VkNuts/Core/Window/*.*")
file(GLOB VKNUTS_MODULE_PRIVATE "source/private/VkNuts/Module/*.*")
file(GLOB VKNUTS_VK_RENDERER_PRIVATE "source/private/VkNuts/VkRenderer/*.*")
file(GLOB VKNUTS_VK_RENDERER_MODULE_PRIVATE "source/private/VkNuts/VkRenderer/Module/*.*")
file(GLOB VKNUTS_VK_RENDERER_MODULE_IMAGE_PRIVATE "source/private/VkNuts/VkRenderer/Module/Image/*.*")
file(GLOB VKNUTS_VK_RENDERER_MODULE_SHADER_PRIVATE "source/private/VkNuts/VkRenderer/Module/Shader/*.*")
file(GLOB VKNUTS_VK_RENDERER_MODULE_TEXTURE_PRIVATE "source/private/VkNuts/VkRenderer/Module/Texture/*.*")
file(GLOB UTILITIES_PUBLIC "source/public/Utilities/*.*")
file(GLOB UTILITIES_PRIVATE "source/private/Utilities/*.*")
file(GLOB UTILITIES_STBIMAGE_PUBLIC "source/public/Utilities/STBImage/*.*")
file(GLOB UTILITIES_STBIMAGE_PRIVATE "source/private/Utilities/STBImage/*.*")
file(GLOB UTILITIES_IMGUI_PUBLIC "source/public/Utilities/ImGui/*.*")
file(GLOB UTILITIES_IMGUI_PRIVATE "source/private/Utilities/ImGui/*.*")
file(GLOB UTILITIES_IMGUI_IMGUILIB_PUBLIC "source/public/Utilities/ImGui/imgui/*.*")
file(GLOB UTILITIES_IMGUI_IMGUILIB_PRIVATE "source/private/Utilities/ImGui/imgui/*.*")
file(GLOB UTILITIES_IMGUI_IMGUILIB_BACKENDS_PUBLIC "source/public/Utilities/ImGui/imgui/backends/*.*")
file(GLOB UTILITIES_IMGUI_IMGUILIB_BACKENDS_PRIVATE "source/private/Utilities/ImGui/imgui/backends/*.*")

file(GLOB_RECURSE VKNUTS_BASE_HEADERS_PRIVATE "source/private/*.h" "source/private/*.hpp")
file(GLOB_RECURSE VKNUTS_BASE_SOURCES_PRIVATE "source/private/*.cpp" "source/private/*.c")
file(GLOB_RECURSE VKNUTS_BASE_HEADERS_PUBLIC "source/public/*.h" "source/public/*.hpp")
file(GLOB_RECURSE VKNUTS_BASE_SOURCES_PUBLIC "source/public/*.cpp" "source/public/*.c")

set(VKNUTS_BASE_INCLUDE_DIRS_PRIVATE "source/private/")
set(VKNUTS_BASE_SOURCE_DIRS_PRIVATE "source/private/")
set(VKNUTS_BASE_INCLUDE_DIRS_PUBLIC "source/public/")
set(VKNUTS_BASE_SOURCE_DIRS_PUBLIC "source/public/")
set(VKNUTS_UTILITIES_IMGUI_IMGUILIB_PUBLIC "source/public/Utilities/ImGui/imgui/")

set(ENTT_INCLUDE_DIR "${EXTERN_DIR}/entt/include/")
set(GLFW_INCLUDE_DIR "${EXTERN_DIR}/glfw/include/")
set(GLM_INCLUDE_DIR "${EXTERN_DIR}/glm/")
set(SPDLOG_INCLUDE_DIR "${EXTERN_DIR}/spdlog/include")
set(STBIMAGE_INCLUDE_DIR "${EXTERN_DIR}/stb/")
set(VULKAN_INCLUDE_DIR "${EXTERN_DIR}/vulkan/")
set(VULKAN_MEM_ALLOC_INCLUDE_DIR "${EXTERN_DIR}/VulkanMemoryAllocator/")

################################################################################
### Visual studio solution
################################################################################
source_group("VkNutsBase/" FILES ${VKNUTSBASE_PUBLIC})
source_group("VkNutsBase/Platform/" FILES ${PLATFORM_PUBLIC})
source_group("VkNutsBase/Platform/" FILES ${PLATFORM_PRIVATE})
source_group("VkNutsBase/Platform/Generic/" FILES ${PLATFORM_GENERIC_PUBLIC})
source_group("VkNutsBase/Platform/Vulkan/" FILES ${PLATFORM_VULKAN_PUBLIC})
source_group("VkNutsBase/Platform/Vulkan/" FILES ${PLATFORM_VULKAN_PRIVATE})
source_group("VkNutsBase/Platform/Vulkan/VkManagement/" FILES ${PLATFORM_VULKAN_VKMANAGEMENT_PUBLIC})
source_group("VkNutsBase/Platform/Vulkan/VkManagement/" FILES ${PLATFORM_VULKAN_VKMANAGEMENT_PRIVATE})
source_group("VkNutsBase/VKNuts/Core/" FILES ${VKNUTS_CORE_PUBLIC})
source_group("VkNutsBase/VKNuts/Core/Attachment/" FILES ${VKNUTS_CORE_ATTACHMENT_PUBLIC})
source_group("VkNutsBase/VKNuts/Core/Engine/" FILES ${VKNUTS_CORE_ENGINE_PRIVATE})
source_group("VkNutsBase/VKNuts/Core/Engine/" FILES ${VKNUTS_CORE_ENGINE_PUBLIC})
source_group("VkNutsBase/VKNuts/Core/Gui/" FILES ${VKNUTS_CORE_GUI_PRIVATE})
source_group("VkNutsBase/VKNuts/Core/Gui/" FILES ${VKNUTS_CORE_GUI_PUBLIC})
source_group("VkNutsBase/VKNuts/Core/Event/" FILES ${VKNUTS_CORE_EVENT_PUBLIC})
source_group("VkNutsBase/VKNuts/Core/Layer/" FILES ${VKNUTS_CORE_LAYER_PUBLIC})
source_group("VkNutsBase/VKNuts/Core/Layer/" FILES ${VKNUTS_CORE_LAYER_PRIVATE})
source_group("VkNutsBase/VKNuts/Core/Log/" FILES ${VKNUTS_CORE_LOG_PUBLIC})
source_group("VkNutsBase/VKNuts/Core/Log/" FILES ${VKNUTS_CORE_LOG_PRIVATE})
source_group("VkNutsBase/VKNuts/Core/MemoryManagement/" FILES ${VKNUTS_CORE_MEMORY_MANAGEMENT_PUBLIC})
source_group("VkNutsBase/VKNuts/Core/MemoryManagement/" FILES ${VKNUTS_CORE_MEMORY_MANAGEMENT_PRIVATE})
source_group("VkNutsBase/VKNuts/Core/Registry/" FILES ${VKNUTS_CORE_REGISTRY_PUBLIC})
source_group("VkNutsBase/VkNuts/Core/Window/" FILES ${VKNUTS_CORE_WINDOW_PRIVATE})
source_group("VkNutsBase/VkNuts/Core/Window/" FILES ${VKNUTS_CORE_WINDOW_PUBLIC})
source_group("VkNutsBase/VKNuts/Module/" FILES ${VKNUTS_MODULE_PRIVATE})
source_group("VkNutsBase/VkNuts/VkRenderer/" FILES ${VKNUTS_VK_RENDERER_PRIVATE})
source_group("VkNutsBase/VkNuts/VkRenderer/Module/" FILES ${VKNUTS_VK_RENDERER_MODULE_PRIVATE})
source_group("VkNutsBase/VkNuts/VkRenderer/Module/Image/" FILES ${VKNUTS_VK_RENDERER_MODULE_IMAGE_PRIVATE})
source_group("VkNutsBase/VkNuts/VkRenderer/Module/Shader/" FILES ${VKNUTS_VK_RENDERER_MODULE_SHADER_PRIVATE})
source_group("VkNutsBase/VkNuts/VkRenderer/Module/Texture/" FILES ${VKNUTS_VK_RENDERER_MODULE_TEXTURE_PRIVATE})
source_group("VkNutsBase/Utilities/" FILES ${UTILITIES_PUBLIC})
source_group("VkNutsBase/Utilities/" FILES ${UTILITIES_PRIVATE})
source_group("VkNutsBase/Utilities/STBImage/" FILES ${UTILITIES_STBIMAGE_PUBLIC})
source_group("VkNutsBase/Utilities/STBImage/" FILES ${UTILITIES_STBIMAGE_PRIVATE})
source_group("VkNutsBase/Utilities/ImGui/" FILES ${UTILITIES_IMGUI_PUBLIC})
source_group("VkNutsBase/Utilities/ImGui/" FILES ${UTILITIES_IMGUI_PRIVATE})
source_group("VkNutsBase/Utilities/ImGui/imgui/" FILES ${UTILITIES_IMGUI_IMGUILIB_PUBLIC})
source_group("VkNutsBase/Utilities/ImGui/imgui/" FILES ${UTILITIES_IMGUI_IMGUILIB_PRIVATE})
source_group("VkNutsBase/Utilities/ImGui/imgui/backends/" FILES ${UTILITIES_IMGUI_IMGUILIB_BACKENDS_PUBLIC})
source_group("VkNutsBase/Utilities/ImGui/imgui/backends/" FILES ${UTILITIES_IMGUI_IMGUILIB_BACKENDS_PRIVATE})

################################################################################
### Add build target
################################################################################
add_library(VkNutsBase SHARED ${VKNUTS_BASE_HEADERS_PRIVATE} ${VKNUTS_BASE_HEADERS_PUBLIC}
                                ${VKNUTS_BASE_SOURCES_PRIVATE} ${VKNUTS_BASE_SOURCES_PUBLIC})
add_dependencies(VkNutsBase glfw spdlog)

################################################################################
### Precompiled headers
################################################################################
if (MSVC_IDE)
    file(GLOB_RECURSE PCH_SOURCES ${VKNUTS_BASE_INCLUDE_DIRS_PUBLIC}/*.cpp ${VKNUTS_BASE_INCLUDE_DIRS_PRIVATE}/*.cpp)
    set_source_files_properties(${PCH_SOURCES} PROPERTIES COMPILE_FLAGS /Yu"NutsPCH.h")
    set_source_files_properties(${VKNUTS_BASE_SOURCE_DIRS_PUBLIC}/NutsPCH.cpp PROPERTIES COMPILE_FLAGS /Yc"NutsPCH.h")
    
    file(GLOB_RECURSE IMGUI_NO_PCH ${UTILITIES_IMGUI_IMGUILIB_PUBLIC}/*.cpp ${UTILITIES_IMGUI_IMGUILIB_PRIVATE}/*.cpp
    ${UTILITIES_IMGUI_IMGUILIB_BACKENDS_PUBLIC}/*.cpp
    ${UTILITIES_IMGUI_IMGUILIB_BACKENDS_PRIVATE}/*.cpp)

    set_source_files_properties(${IMGUI_NO_PCH} PROPERTIES COMPILE_FLAGS "")
endif()

################################################################################
### Configure target
################################################################################
target_include_directories(VkNutsBase PUBLIC ${VKNUTS_BASE_INCLUDE_DIRS_PUBLIC} ${ENTT_INCLUDE_DIR}
                                        ${GLFW_INCLUDE_DIR} ${GLM_INCLUDE_DIR}
                                        ${IMGUI_INCLUDE_DIR} ${SPDLOG_INCLUDE_DIR}
                                        ${VULKAN_INCLUDE_DIR} ${VULKAN_MEM_ALLOC_INCLUDE_DIR})
target_include_directories(VkNutsBase PRIVATE ${VKNUTS_BASE_INCLUDE_DIRS_PRIVATE} ${STBIMAGE_INCLUDE_DIR} 
                                                ${VKNUTS_UTILITIES_IMGUI_IMGUILIB_PUBLIC})

target_link_libraries(VkNutsBase PUBLIC 
                            glfw
                            spdlog
                            "${VULKAN_INCLUDE_DIR}Lib/vulkan-1.lib" 
                            "${VULKAN_INCLUDE_DIR}Lib/VkLayer_utils.lib" )

target_compile_definitions(VkNutsBase PRIVATE -DNUTS_BUILD_DLL -DNUTS_BUILD_ENGINE)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(VkNutsBase PRIVATE -DNUTS_ENABLE_LOG)
else()
    message("Logging is disabled for Release builds")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(VkNutsBase PRIVATE 
                                    -Wall -fexceptions -std=c++20 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -Oo)
    target_link_options(VkNutsBase PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(VkNutsBase PRIVATE 
                                    -Wall -fexceptions -std=c++20 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3)
    target_link_options(VkNutsBase PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(VkNutsBase PRIVATE 
                                    -Wall -fexceptions -std=c++20 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -Oo)
    target_link_options(VkNutsBase PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(VkNutsBase PRIVATE 
                                    -Wall -fexceptions -std=c++20 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3)
    target_link_options(VkNutsBase PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(VkNutsBase PRIVATE 
                                    /W4 /WX- /permissive- /std:c++20
                                    /TP /Gd /Gm- /RTC1 /Od /diagnostics:classic
                                    /fp:precise /EHsc)
    target_link_options(VkNutsBase PRIVATE /INCREMENTAL /MACHINE:X64)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(VkNutsBase PRIVATE 
                                    /W4 /WX- /permissive- /std:c++20
                                    /TP /Gd /Gm- /diagnostics:classic
                                    /fp:precise /EHsc /Ob2 /O2 /Oi)
    target_link_options(VkNutsBase PRIVATE /INCREMENTAL /MACHINE:X64)
endif()
