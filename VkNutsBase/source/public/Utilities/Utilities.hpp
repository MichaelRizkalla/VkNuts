#pragma once

#include <Platform/CompilerDefines.h>
#include <Platform/PlatformDefines.h>
#include <type_traits>

#if defined(NUTS_OS_WINDOWS)
    #include <windows.h>
#elif defined(NUTS_OS_LINUX)
    #include <dlfcn.h>
#else
    #error "OS is not yet supported!"
#endif

namespace nuts {
    struct array_size {
        template < typename Type, size_t N >
        static constexpr auto get(Type (&)[N]) {
            return N;
        }

        template < typename OutType, typename Type, size_t N >
        static constexpr auto get(Type (&)[N]) {
            return static_cast< OutType >(N);
        }
    };

    template < typename TEnum >
    struct enum_size {
        static constexpr std::underlying_type_t< TEnum > value = static_cast< std::underlying_type_t< TEnum > >(TEnum::NUTS_ENUM_COUNT);
    };
    template < typename TEnum >
    static constexpr auto enum_size_v = enum_size< TEnum >::value;

    template < typename Type >
    struct contains_data_function {
      private:
        template < typename TypeChecked >
        static constexpr auto check(TypeChecked*)
            -> decltype(std::is_same_v< typename TypeChecked::value_type*, decltype(std::declval< TypeChecked& >().data()) >, std::true_type {});

        template < typename >
        static constexpr std::false_type check(...);

      public:
        static constexpr auto value = decltype(check< Type >(0))::value;
    };

    template < typename Type >
    static constexpr auto contains_data_function_v = contains_data_function< Type >::value;

    template < typename Type >
    concept has_data_function = contains_data_function_v< Type >;

    template < typename Type >
    struct contains_resize_function {
      private:
        template < typename TypeChecked >
        static constexpr auto check(TypeChecked*)
            -> decltype(std::is_same_v< void, decltype(std::declval< TypeChecked& >().resize(std::declval< typename TypeChecked::size_type& >())) >, std::true_type {});

        template < typename >
        static constexpr std::false_type check(...);

      public:
        static constexpr auto value = decltype(check< Type >(0))::value;
    };

    template < typename Type >
    static constexpr auto contains_resize_function_v = contains_resize_function< Type >::value;

    template < typename Type >
    concept has_resize_function = contains_resize_function_v< Type >;

    struct FunctionLoader {
        static auto load(void* handle, const char* functionName) noexcept {
#if defined(NUTS_OS_WINDOWS)
            return ::GetProcAddress(static_cast< HMODULE >(handle), functionName);
#elif defined(NUTS_OS_LINUX)
            return dlsym(handle, functionName);
#else
    #error "OS not yet supported!"
#endif
        }
    };

} // namespace nuts