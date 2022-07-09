#pragma once
#include <functional>
#include <memory>
#include <memory_resource>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace nuts {

    template < class Type, class Del = std::default_delete< Type > >
    using UniqueRef = std::unique_ptr< Type, Del >;

    template < class Type = std::byte >
    using NutsAllocator = std::pmr::polymorphic_allocator< Type >;

    template < class Type >
    using SharedRef = std::shared_ptr< Type >;

    template < class Key, class Value, class Hasher = std::hash< Key >, class Compare = std::equal_to< Key >,
               class Allocator = NutsAllocator< std::pair< Key const, Value > > >
    using HashMap = std::unordered_map< Key, Value, Hasher, Compare, Allocator >;

    template < class Type, class Traits = std::char_traits< Type >, class Allocator = NutsAllocator< Type > >
    using BasicString = std::basic_string< Type, Traits, Allocator >;

    using String = BasicString< char >;

    template < class Type, class Allocator = NutsAllocator< Type > >
    using Vector = std::vector< Type, Allocator >;

} // namespace nuts
