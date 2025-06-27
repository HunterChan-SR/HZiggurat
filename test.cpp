// test.cpp
#include "ziggurat256_test.hpp"
#include "ziggurat256_secret_test.hpp"

// #include <cmath>
// #include <cstdint>
// #include <random>
// #include <vector>
// #include <iostream>

int main()
{
    test::testZiggurat256();

    puts("");

    test::testZiggurat256_secret();

    puts("");
    return 0;
}