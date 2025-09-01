#pragma once

#include <algorithm>
#include <concepts>
#include <compare>

namespace lib {

    template <typename T, std::size_t N>
    requires std::is_move_constructible_v<T> && std::is_move_assignable_v<T>
    struct array {
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;

        using iterator = T*;
        using const_iterator = const T*;

        T data_[N];

        constexpr T& at(std::size_t index) noexcept {
            static_assert(index >= 0 && index < N, "array index out of bounds");
            return data_[index];
        }

        constexpr T& operator[](std::size_t index) noexcept { return data_[index]; }
        constexpr const T& operator[](std::size_t index) const noexcept { return data_[index]; }

        constexpr T& front() noexcept { return data_[0]; }
        constexpr const T& front() const noexcept { return data_[0]; }

        constexpr T& back() noexcept { return data_[N-1]; }
        constexpr const T& back() const noexcept { return data_[N-1]; }

        constexpr T* data() noexcept { return data_; }
        constexpr const T* data() const noexcept { return data_; }

        constexpr iterator begin() noexcept { return data_; }
        constexpr iterator end() noexcept { return data_ + N; }

        constexpr const_iterator cbegin() const noexcept { return data_; }
        constexpr const_iterator cend() const noexcept { return data_ + N; }

        constexpr size_type get_size() const noexcept { return N; }
        constexpr size_type max_size() const noexcept { return N; }
        constexpr bool empty() const noexcept { return N == 0; }
        
        constexpr void fill(const T& value) { 
            std::fill(data_, data_ + N, value);
        }

        friend constexpr void swap(array& a, array& b) noexcept {
            std::swap_ranges(a.data_, a.data_ + N, b.data_);
        }
    };

    template <typename T, std::size_t N>
    auto operator<=>(const array<T, N>& lhs, const array<T, N>& rhs) {
        return std::lexicographical_compare_three_way(
            lhs.data(), lhs.data() + N,
            rhs.data(), rhs.data() + N
        );
    }

    template <typename T, std::size_t N>
    auto operator==(const array<T, N>& lhs, const array<T, N>& rhs) {
        return std::lexicographical_compare(
            lhs.data(), lhs.data() + N,
            rhs.data(), rhs.data() + N
        );
    }

}



