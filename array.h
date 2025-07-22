#include <cstdint>
#include <initializer_list>

template <typename T, int64_t size>
class array {
public:
    array() : ptr_{ new T[size] } {}

    array(std::initializer_list<T> items) : ptr_{ new T[size] } {
        for (int64_t i = 0; i < items.size(); i++) {
            ptr_[i] = items[i];
        }
    }





    T* ptr_{ nullptr };
};
