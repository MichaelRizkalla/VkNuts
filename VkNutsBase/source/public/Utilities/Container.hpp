#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/TypeAlias.h>

namespace nuts {
    /// <summary>
    /// The container can construct "in_place" any ContainedType if std::is_default_constructible_v is true
    /// Otherwise, the allocated space will be un-initialized.
    /// </summary>
    /// <typeparam name="ContainedType"></typeparam>
    /// <typeparam name="Allocator"></typeparam>
    template < typename ContainedType, class Allocator = NutsAllocator< std::byte > >
    class Container {
      public:
        using value_type          = ContainedType;
        using size_type           = std::size_t;
        using reference_type      = ContainedType&;
        using iterator_type       = ContainedType*;
        using const_iterator_type = const ContainedType*;
        using allocator_type      = std::allocator_traits< Allocator >::template rebind_alloc< ContainedType >;

        constexpr Container() = default;
        constexpr Container(Allocator al) : Al(allocator_type { al }) {
        }
        constexpr Container(Allocator al, size_type size) : Al(allocator_type { al }) {
            Allocate(Size);
        }
        /// <summary>
        /// TODO: Add in_place_construct, and no_in_place_construct for better object management flags
        /// </summary>
        /// <param name="size"></param>
        constexpr Container(size_type size) {
            Allocate(size);
        }
        constexpr Container(const Container& rhs) : Al(rhs.Al), Size(rhs.Size) {
            Allocate(Size);
            std::memcpy(Data, rhs.Data, sizeInBytes());
        }
        constexpr Container(Container&& rhs) noexcept : Al(std::move(rhs.Al)), Size(rhs.Size) {
            Data     = std::exchange(rhs.Data, nullptr);
            rhs.Size = 0;
        }
        constexpr Container& operator=(const Container& rhs) {
            if (this != std::addressof(rhs)) {
                Deallocate();
                if constexpr (std::allocator_traits< allocator_type >::is_always_equal::value ||
                              std::allocator_traits< allocator_type >::propagate_on_container_copy_assignment::value) {
                    Al = rhs.Al;
                }
                Size = rhs.Size;
                Allocate(Size);
                std::memcpy(Data, rhs.Data, sizeInBytes());
            }
            return *this;
        }
        constexpr Container& operator=(Container&& rhs) noexcept(false) {
            if (this != std::addressof(rhs)) {
                Deallocate();
                if constexpr (std::allocator_traits< allocator_type >::is_always_equal::value) {
                    Al   = std::move(rhs.Al);
                    Data = std::exchange(rhs.Data, nullptr);
                    Size = std::exchange(rhs.Size, 0);
                } else if constexpr (std::allocator_traits< allocator_type >::propagate_on_container_move_assignment::value) {
                    Al   = std::move(rhs.Al);
                    Data = std::exchange(rhs.Data, nullptr);
                    Size = std::exchange(rhs.Size, 0);
                } else {
                    if (Al == rhs.Al) {
                        Data = std::exchange(rhs.Data, nullptr);
                        Size = std::exchange(rhs.Size, 0);
                    } else {
                        Allocate(Size);
                        std::memcpy(Data, rhs.Data, sizeInBytes());
                        rhs.Deallocate();
                    }
                }
            }
            return *this;
        }
        constexpr ~Container() {
            if (Data)
                Deallocate();
        }

        /// <summary>
        /// Initialize un-initialize allocated memory at specific-index
        /// </summary>
        /// <typeparam name="...Types"></typeparam>
        /// <param name="index"></param>
        /// <param name="...args"></param>
        template < class... Types >
        inline constexpr void construct_at(size_type index, Types&&... args) {
            std::allocator_traits< allocator_type >::construct(Al, Data + index, args...);
        }
        inline constexpr ContainedType* data() noexcept {
            return Data;
        }
        inline constexpr const ContainedType* data() const noexcept {
            return Data;
        }
        inline constexpr size_type size() const noexcept {
            return Size;
        }
        inline constexpr size_type sizeInBytes() const noexcept {
            return Size * TYPE_SIZE;
        }
        inline constexpr void resize(size_type newSize) {
            Deallocate();
            Allocate(newSize);
        }
        inline constexpr void reset() noexcept {
            Deallocate();
        }
        inline constexpr iterator_type begin() noexcept {
            return &Data[0];
        }
        inline constexpr iterator_type end() noexcept {
            return &Data[Size];
        }
        inline constexpr const_iterator_type begin() const noexcept {
            return &Data[0];
        }
        inline constexpr const_iterator_type end() const noexcept {
            return &Data[Size];
        }
        inline constexpr iterator_type find(const value_type& value) {
            return std::find(begin(), end(), value);
        }
        inline constexpr const_iterator_type find(const value_type& value) const {
            return std::find(begin(), end(), value);
        }
        inline constexpr bool contains(const value_type& value) {
            return std::find(begin(), end(), value) != end();
        }
        inline constexpr bool contains(const value_type& value) const {
            return std::find(begin(), end(), value) != end();
        }
        inline constexpr value_type& operator[](size_type index) {
            return Data[index];
        }
        inline constexpr const value_type& operator[](size_type index) const {
            return Data[index];
        }
        inline constexpr value_type& at(size_type index) {
            if (Size <= index) {
                throw std::out_of_range("invalid Container subscript");
            }
            return Data[index];
        }
        inline constexpr const value_type& at(size_type index) const {
            if (Size <= index) {
                throw std::out_of_range("invalid Container subscript");
            }
            return Data[index];
        }

      private:
        inline constexpr void Allocate(size_type size) {
            Size = size;
            Data = Al.allocate(size);

            if constexpr (std::is_default_constructible_v< ContainedType >) {
                for (auto i = 0; i < Size; i++) {
                    std::allocator_traits< allocator_type >::construct(Al, Data + i);
                }
            }
        }
        inline constexpr void Deallocate() {
            if (Data) {
                Al.deallocate(Data, Size);
                Data = nullptr;
            }
            Size = 0;
        }

        static constexpr size_type TYPE_SIZE = sizeof(ContainedType);
        std::size_t                Size { 0 };
        ContainedType*             Data { nullptr };
        allocator_type             Al {}; /* use no_unique_address when available */
    };

    template < class ContainedType >
    [[nodiscard]] inline constexpr auto begin(Container< ContainedType >& container) noexcept {
        return container.begin();
    }
    template < class ContainedType >
    [[nodiscard]] inline constexpr auto end(Container< ContainedType >& container) noexcept {
        return container.end();
    }
    template < class ContainedType >
    [[nodiscard]] inline constexpr auto begin(const Container< ContainedType >& container) noexcept {
        return container.begin();
    }
    template < class ContainedType >
    [[nodiscard]] inline constexpr auto end(const Container< ContainedType >& container) noexcept {
        return container.end();
    }
} // namespace nuts
