#pragma once

#include <iostream>
#include "ziggurat256.hpp"


namespace test{
    void testZiggurat256(){
        ziggurat256 zig;
        zig.debug_print();
        zig.chceck();
        for (int i = 0; i < 10; ++i){
            std::cout << zig.next_normal() << " ";
        }
    }
}

