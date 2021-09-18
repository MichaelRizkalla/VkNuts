################################################################################
### Programming language version and compiler flags
################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(TestRun CXX)

################################################################################
### Includes - source files, header files, moc files
################################################################################
set(EXTERN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../extern")
set(ENGINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../VkNutsBase/source/")

file(GLOB TEST_RUN "source/*.*")

file(GLOB_RECURSE TEST_RUN_HEADERS "source/*.h" "source/*.hpp")
file(GLOB_RECURSE TEST_RUN_SOURCES "source/*.cpp" "source/*.c")

set(TEST_RUN_INCLUDE_DIRS "source/")
set(TEST_RUN_SOURCE_DIRS "source/")

################################################################################
### Add build target
################################################################################
add_executable(TestRun ${TEST_RUN_SOURCES} ${TEST_RUN_HEADERS})
add_dependencies(TestRun VkNutsBase)

################################################################################
### Precompiled headers - not used
################################################################################
#if (MSVC_IDE)
    #file(GLOB PCH_SOURCES ${TEST_RUN_SOURCE_DIRS}/*.cpp)
    #set_source_files_properties(${PCH_SOURCES} PROPERTIES COMPILE_FLAGS /Yu"pch.h")
    #set_source_files_properties(${TEST_RUN_SOURCE_DIRS}/pch.cpp PROPERTIES COMPILE_FLAGS /Yc"pch.h")
#endif()

################################################################################
### Visual studio solution - no groups yet
################################################################################
#source_group("VkNuts/" FILES ${VKNUTS})

################################################################################
### Configure target
################################################################################
target_include_directories(TestRun PRIVATE ${TEST_RUN_INCLUDE_DIRS} ${ENGINE_DIR})

target_link_libraries(TestRun VkNutsBase)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(TestRun PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -Oo)
    target_link_options(TestRun PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(TestRun PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3)
    target_link_options(TestRun PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(TestRun PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -Oo)
    target_link_options(TestRun PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(TestRun PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3)
    target_link_options(TestRun PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(TestRun PRIVATE 
                                    /W4 /WX- /permissive- /std:c++17
                                    /TP /Gd /Gm- /RTC1 /Od /diagnostics:classic
                                    /fp:precise /EHsc)
    target_link_options(TestRun PRIVATE /INCREMENTAL /MACHINE:X64)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(TestRun PRIVATE 
                                    /W4 /WX- /permissive- /std:c++17
                                    /TP /Gd /Gm- /diagnostics:classic
                                    /fp:precise /EHsc /Ob2 /Ox /Oi)
    target_link_options(TestRun PRIVATE /INCREMENTAL /MACHINE:X64)
endif()
