#include <print>
#include <array>
#include <ranges>

#include "array.h"

int main() {
    lib::array<int, 3> a{ 1, 2, 3 };
    lib::array<int, 3> b{ 4, 5, 6 };
    lib::array<int, 3> c;
    
    swap(a, b);

    for (auto& x : a) { std::print("{} ", x); }
    std::println();
    for (auto& x : b) { std::print("{} ", x); }
    std::println();
    for (auto& x : c) { std::print("{} ", x); }
    std::println();

}
