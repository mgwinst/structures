#include <print>
#include <array>
#include <ranges>

#include "array.h"

struct Value {
    int x, y;
};


int main() {
    array<Value, 3> a{ Value{1, 2}, Value{4, 5}, Value{7, 8}};
    array<Value, 3> b{ a };

    for (auto x : b) {
        std::print("({},{}) ", x.x, x.y);
    }

}
