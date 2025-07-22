#include <print>

#include "array.h"


int main() {
    array<int, 10> a { 1, 2, 3 };
    
    for (int i = 0; i < 10; i++) {
        std::print("{} ", a.ptr[i]);
    }
    std::println();
}
