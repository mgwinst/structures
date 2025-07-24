#include <print>
#include <ranges>

#include "array.h"

using namespace lib;

int main() {
    array<int, 3> a{ 1, 3, 3 };
    array<int, 3> b{ 1, 2, 4 };

    auto result = a <=> b;

    if (result < 0) { std::print("a < b\n"); };
    if (result == 0) { std::print("a == b\n"); };
    if (result > 0) { std::print("a > b\n"); };

}
