#pragma once
#include <functional>
#include <memory>
#include <memory_resource>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace nuts {

    template < class Type, class Del = std::function< void(Type*) > >
    using UniqueRef = std::unique_ptr< Type, Del >;

    template < class Type, class Allocator, class... TArgs, std::enable_if_t< !std::is_array_v< Type >, int > = 0 >
    UniqueRef< Type, std::function< void(Type*) > > allocate_unique(Allocator alloc, TArgs... args) {
        using allocator_type = std::allocator_traits< Allocator >::template rebind_alloc< Type >;

        auto custom_deleter = [](Type* ptr, allocator_type m_alloc) {
            std::allocator_traits< allocator_type >::destroy(m_alloc, ptr);
            m_alloc.deallocate(ptr, 1);
        };

        allocator_type type_alloc { alloc };

        Type* ptr = type_alloc.allocate(1);
        std::allocator_traits< allocator_type >::construct(alloc, ptr, std::forward< TArgs >(args)...);

        return { ptr, std::bind(custom_deleter, std::placeholders::_1, type_alloc) };
    }

    template < class Type, class ConstructedType, class Allocator, class... TArgs, std::enable_if_t< !std::is_array_v< Type >, int > = 0 >
    std::unique_ptr< Type, std::function< void(Type*) > > allocate_unique(Allocator alloc, TArgs... args) {
        using constructor = std::allocator_traits< Allocator >::template rebind_alloc< ConstructedType >;

        auto custom_deleter = [](Type* ptr, constructor c_alloc) {
            std::allocator_traits< constructor >::destroy(c_alloc, reinterpret_cast< ConstructedType* >(ptr));
            c_alloc.deallocate(reinterpret_cast< ConstructedType* >(ptr), 1);
        };

        constructor ctor_alloc { alloc };

        ConstructedType* ptr = ctor_alloc.allocate(1);
        std::allocator_traits< constructor >::construct(ctor_alloc, ptr, std::forward< TArgs >(args)...);

        return { static_cast< Type* >(ptr), std::bind(custom_deleter, std::placeholders::_1, ctor_alloc) };
    }

    template < class ArrayType, class Allocator, std::enable_if_t< std::is_array_v< ArrayType > && std::extent_v< ArrayType > == 0, int > = 0 >
    UniqueRef< ArrayType, std::function< void(std::remove_extent_t< ArrayType >*) > > allocate_unique(Allocator alloc, const std::size_t size) {
        using Type           = std::remove_extent_t< ArrayType >;
        using allocator_type = std::allocator_traits< Allocator >::template rebind_alloc< Type >;

        auto custom_array_deleter = [](Type* ptr, allocator_type m_alloc, std::size_t m_size) {
            for (std::size_t i = 0; i < m_size; ++i) {
                std::allocator_traits< allocator_type >::destroy(m_alloc, ptr + i);
            }
            m_alloc.deallocate(ptr, m_size);
        };

        allocator_type type_alloc { alloc };

        Type* ptr = type_alloc.allocate(size);
        if constexpr (std::is_default_constructible_v< Type >) {
            for (std::size_t i = 0; i < size; ++i) {
                std::allocator_traits< allocator_type >::construct(alloc, ptr + i);
            }
        }

        return { ptr, std::bind(custom_array_deleter, std::placeholders::_1, type_alloc, size) };
    }

    template < class Type >
    using SharedRef = std::shared_ptr< Type >;

    template < class Key, class Value, class Hasher = std::hash< Key >, class Compare = std::equal_to< Key >,
               class Allocator = std::pmr::polymorphic_allocator< std::pair< Key const, Value > > >
    using HashMap = std::unordered_map< Key, Value, Hasher, Compare, Allocator >;

    template < class Type, class Traits = std::char_traits< Type >, class Allocator = std::pmr::polymorphic_allocator< Type > >
    using BasicString = std::basic_string< Type, Traits, Allocator >;

    using String = BasicString< char >;

    template < class Type, class Allocator = std::pmr::polymorphic_allocator< Type > >
    using Vector = std::vector< Type, Allocator >;
} // namespace nuts