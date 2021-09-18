#if defined(_WIN32) && defined(_WIN64)
    #define NUTS_OS_WINDOWS_X64
#elif defined(_WIN32)
    #define NUTS_OS_WINDOWS_X86
#elif defined(__linux__)
    #error "Linux is not yet supported!"
    #define NUTS_OS_LINUX
#elif defined(__ANDROID__)
    #error "Android is not yet supported!"
    #define NUTS_OS_ANDROID
#else
    #error "Undefined-unsupported platform"
#endif

#if defined(NUTS_OS_WINDOWS_X64) || defined(NUTS_OS_WINDOWS_X86)
    #define NUTS_OS_WINDOWS
#endif

#if defined(NUTS_OS_WINDOWS)
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMCX
        #define NOMCX
    #endif
    #ifndef NOIME
        #define NOIME
    #endif
#endif

#if defined(NUTS_OS_WINDOWS)
    #include <Windows.h>
#elif defined(NUTS_OS_LINUX)
    #include <dlfcn.h>
#else
    #error "OS is not yet supported!"
#endif
