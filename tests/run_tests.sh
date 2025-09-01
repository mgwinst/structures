clang++ -std=c++23 -I../ \
    test_main.cc test_array.cc \
    -lgtest -lgtest_main && ./a.out
