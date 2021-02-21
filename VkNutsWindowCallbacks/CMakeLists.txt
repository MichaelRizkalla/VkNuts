################################################################################
### Programming language version and compiler flags
################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(vknuts-windowcallback CXX)

################################################################################
### Includes - source files, header files, moc files
################################################################################
set(EXTERN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../extern")
set(VKNUTS_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../VkNutsBase")

file(GLOB LIB_HEADER "source/*.h" "source/*.hpp")
file(GLOB LIB_SOURCE "source/*.cpp" "source/*.c")

set(LIB_HEADER_DIR "source/")
set(VKNUTS_PUBLIC_HEADER_DIR "${VKNUTS_BASE_DIR}/source/public/")

set(GLFW_INCLUDE_DIR "${EXTERN_DIR}/glfw/include/")

################################################################################
### Visual studio solution
################################################################################
source_group("vknuts-windowcallback/source/" FILES ${LIB_HEADER} ${LIB_SOURCE})

################################################################################
### Add build target
################################################################################
add_library(vknuts-windowcallback SHARED ${LIB_SOURCE} ${LIB_HEADER})
add_dependencies(vknuts-windowcallback glfw)

################################################################################
### Configure target
################################################################################
target_include_directories(vknuts-windowcallback PRIVATE ${LIB_HEADER_DIR} ${GLFW_INCLUDE_DIR} ${VKNUTS_PUBLIC_HEADER_DIR})

target_compile_definitions(vknuts-windowcallback PRIVATE -DNUTS_BUILD_DLL)

target_link_libraries(vknuts-windowcallback PRIVATE glfw)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(VkNutsBase PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -Oo)
    target_link_options(VkNutsBase PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(VkNutsBase PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3)
    target_link_options(VkNutsBase PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(VkNutsBase PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -Oo)
    target_link_options(VkNutsBase PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(VkNutsBase PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3)
    target_link_options(VkNutsBase PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(VkNutsBase PRIVATE 
                                    /W4 /WX- /permissive- /std:c++17
                                    /TP /Gd /Gm- /RTC1 /Od /diagnostics:classic
                                    /fp:precise /EHsc)
    target_link_options(VkNutsBase PRIVATE /INCREMENTAL /MACHINE:X64)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(VkNutsBase PRIVATE 
                                    /W4 /WX- /permissive- /std:c++17
                                    /TP /Gd /Gm- /diagnostics:classic
                                    /fp:precise /EHsc /Ob2 /Ox /Oi)
    target_link_options(VkNutsBase PRIVATE /INCREMENTAL /MACHINE:X64)
endif()