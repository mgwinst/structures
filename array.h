#include <initializer_list>
#include <algorithm>
#include <utility>

template <typename T, std::size_t size>
struct array {
    T data_[size];
    
    constexpr T& at(std::size_t index) noexcept {
        static_assert(index >= 0 && index < size, "array index out of bounds");
        return data_[index];
    }

    constexpr T& operator[](std::size_t index) noexcept { return data_[index]; }
    constexpr T& front() noexcept { return data_[0]; }
    constexpr T& back() noexcept { return data_[size-1]; }
    constexpr const T& operator[](std::size_t index) const noexcept { return data_[index]; }
    constexpr const T& front() const noexcept { return data_[0]; }
    constexpr const T& back() const noexcept { return data_[size-1]; }

    constexpr T* data() noexcept { return data_; }
    constexpr const T* data() const noexcept { return data_; }

    constexpr T* begin() noexcept { return data_; }
    constexpr T* end() noexcept { return data_ + size; }
    constexpr const T* cbegin() const noexcept { return data_; }
    constexpr const T* cend() const noexcept { return data_ + size; }

    constexpr std::size_t get_size() const noexcept { return size; }
    constexpr bool empty() const noexcept { return size == 0; }
    

    constexpr void fill(const T& value) { 
        std::fill(data_, data_ + size, value);
    }

    friend constexpr void swap(const array& a, const array& b) {
        std::swap_ranges(a.data_, a.data_ + size, b.data_);
    }

    ~array() = default;
};

