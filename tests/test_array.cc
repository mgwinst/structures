#include <gtest/gtest.h>

#include "array.h"

using namespace lib;

TEST(array, initialization) {
    array<int, 5> a{ 1, 2, 3, 4, 5 };
    int val = 1;
    for (auto x : a) { ASSERT_EQ(x, val++); }
}

TEST(array, fill) {
    array<int, 5> a;
    a.fill(10);
    for (auto x : a) { ASSERT_EQ(x, 10); }
}

TEST(array, swap) {
    array<int, 3> a{ 42, 42, 42 };
    array<int, 3> b{ 133, 133, 133 };

    swap(a, b);
    
    for (auto x : a) { ASSERT_EQ(x, 133); }   
    for (auto x : b) { ASSERT_EQ(x, 42); }   
}



