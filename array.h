#include <initializer_list>
#include <algorithm>
#include <utility>
#include <concepts>

namespace lib {

    template <typename T, std::size_t N>
    requires std::is_move_constructible_v<T> && std::is_move_assignable_v<T>
    struct array {
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

        constexpr T* begin() noexcept { return data_; }
        constexpr T* end() noexcept { return data_ + N; }

        constexpr const T* cbegin() const noexcept { return data_; }
        constexpr const T* cend() const noexcept { return data_ + N; }

        constexpr std::size_t get_size() const noexcept { return N; }
        constexpr std::size_t max_size() const noexcept { return N; }
        constexpr bool empty() const noexcept { return N == 0; }
        
        constexpr void fill(const T& value) { 
            std::fill(data_, data_ + N, value);
        }

        friend constexpr void swap(array& a, array& b) noexcept {
            std::swap_ranges(a.data_, a.data_ + N, b.data_);
        }

        ~array() = default;
    };

}


