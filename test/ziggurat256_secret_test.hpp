#pragma once

#include <iostream>
#include "ziggurat256_secret.hpp"


namespace test{
    void testZiggurat256_secret(){
        ziggurat256_secret zig;
        zig.debug_print();
        zig.chceck();
        for (int i = 0; i < 10; ++i){
            std::cout << zig.next_normal() << " ";
        }
        zig.debug_block();
    }
}

