#if defined(_WIN32)
    #if defined(_MSC_VER) && (_MSC_VER < 1910)
        #error This is a MSVC17 or later compiler header
    #elif defined(_MSC_VER)
        #define NUTS_COMPILER_MSVC
        #define NUTS_INTERFACE_ONLY __declspec(novtable)
        #define NUTS_DLL(cond) __declspec(cond)
    #elif defined(__GNUC__)
        #define NUTS_COMPILER_GNUC
        #define NUTS_INTERFACE_ONLY 
        #define NUTS_DLL(cond) __attribute__((cond))
    #else
        #error "Unsupported compiler"
    #endif
#elif defined(__linux)
    #if defined(_MSC_VER)
        #error "MSVC compiler is not supported on Linux"
    #elif defined(__GNUC__)
        #define NUTS_COMPILER_GNUC                             
        #define NUTS_INTERFACE_ONLY 
        #define NUTS_DLL(cond) __attribute__((visibility("default")))
    #else
        #error "Unsupported compiler"
    #endif
#else
    #error "Unsupport platform"
#endif

#define NUTS_EXPORT NUTS_DLL(dllexport)
#define NUTS_IMPORT NUTS_DLL(dllimport)

#if defined(NUTS_BUILD_DLL)
#define NUTS_API NUTS_EXPORT
#define NUTS_EXPORT_API 1
#else
#define NUTS_API NUTS_IMPORT
#define NUTS_IMPORT_API 1
#endif