#include <initializer_list>
#include <algorithm>
#include <utility>

template <typename T, std::size_t size>
class array {
public:
    array() : ptr_{ new T[size] } {}

    template <std::size_t N>
    array(const T (&arr)[N]) : ptr_{ new T[size] } {
        std::copy(arr, arr + N, ptr_);
    }

    array(const array& arr) {
        ptr_ = new T[size];
        if (!ptr_) throw std::bad_alloc{};
        for (std::size_t i = 0; i < size; i++) {
            ptr_[i] = arr.ptr_[i];
        }
    }

    array& operator=(array arr) {
        swap(*this, arr);
        return *this;
    }

    friend void swap(array& a, array& b) noexcept {
        std::swap(a.ptr_, b.ptr_);
    }

    array(array&& arr) noexcept {
        ptr_ = std::exchange(arr.ptr_, nullptr);
    }

    array& operator=(array&& arr) noexcept {
        delete[] ptr_;
        ptr_ = std::exchange(arr.ptr_, nullptr);
        return *this;
    }

    ~array() { delete[] ptr_; }

private:
    T* ptr_{ nullptr };
};

