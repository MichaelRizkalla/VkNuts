#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

namespace nuts {
    template < typename Key, typename Value, std::size_t mapSize >
    class ConstexprMap {
      public:
        using first_type  = Key;
        using second_type = Value;

        constexpr explicit ConstexprMap(const std::array< std::pair< first_type, second_type >, mapSize >& input_data) : Data(input_data) {}

        [[nodiscard]] inline constexpr second_type at(const first_type& key) const {
            auto result = find(std::move(key));
            if (result != Data.end()) {
                return result->second;
            } else {
                throw std::out_of_range("Element was not found!");
            }
        }
        [[nodiscard]] inline constexpr first_type at(const second_type& key) const {
            auto result = find(std::move(key));
            if (result != Data.end()) {
                return result->first;
            } else {
                throw std::out_of_range("Element was not found!");
            }
        }
        [[nodiscard]] inline constexpr bool contains(const first_type& key) const noexcept {
            auto result = std::find_if(Data.begin(), Data.end(), [&](auto&& pair_data) {
                if (pair_data.first == key) return true;
                return false;
            });
            if (result != Data.end()) { return true; }
            return false;
        }
        [[nodiscard]] inline constexpr bool contains(const second_type& key) const noexcept {
            auto result = std::find_if(Data.begin(), Data.end(), [&](auto&& pair_data) {
                if (pair_data.second == key) return true;
                return false;
            });
            if (result != Data.end()) { return true; }
            return false;
        }
        [[nodiscard]] inline constexpr auto size() const noexcept { return Data.size(); };
        [[nodiscard]] inline constexpr auto begin() noexcept { return Data.begin(); }
        [[nodiscard]] inline constexpr auto end() noexcept { return Data.end(); }
        [[nodiscard]] inline constexpr auto begin() const noexcept { return Data.begin(); }
        [[nodiscard]] inline constexpr auto end() const noexcept { return Data.end(); }
        [[nodiscard]] inline constexpr auto find(const first_type& key) const noexcept {
            return std::find_if(Data.begin(), Data.end(), [&](auto&& pair_data) {
                if (pair_data.first == key) return true;
                return false;
            });
        }
        [[nodiscard]] inline constexpr auto find(const second_type& key) const noexcept {
            return std::find_if(Data.begin(), Data.end(), [&](auto&& pair_data) {
                if (pair_data.second == key) return true;
                return false;
            });
        }
        [[nodiscard]] inline constexpr auto find(const first_type& key) noexcept {
            return std::find_if(Data.begin(), Data.end(), [&](auto&& pair_data) {
                if (pair_data.first == key) return true;
                return false;
            });
        }
        [[nodiscard]] inline constexpr auto find(const second_type& key) noexcept {
            return std::find_if(Data.begin(), Data.end(), [&](auto&& pair_data) {
                if (pair_data.second == key) return true;
                return false;
            });
        }

      private:
        std::array< std::pair< Key, Value >, mapSize > Data;
    };

    template < class FirstType, class SecondType, std::size_t mapSize >
    [[nodiscard]] inline constexpr auto begin(ConstexprMap< FirstType, SecondType, mapSize >& const_map) noexcept {
        return const_map.begin();
    }
    template < class FirstType, class SecondType, std::size_t mapSize >
    [[nodiscard]] inline constexpr auto end(ConstexprMap< FirstType, SecondType, mapSize >& const_map) noexcept {
        return const_map.end();
    }
    template < class FirstType, class SecondType, std::size_t mapSize >
    [[nodiscard]] inline constexpr auto begin(const ConstexprMap< FirstType, SecondType, mapSize >& const_map) noexcept {
        return const_map.begin();
    }
    template < class FirstType, class SecondType, std::size_t mapSize >
    [[nodiscard]] inline constexpr auto end(const ConstexprMap< FirstType, SecondType, mapSize >& const_map) noexcept {
        return const_map.end();
    }
} // namespace nuts
