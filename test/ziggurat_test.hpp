#pragma once

#include <iostream>
#include "ziggurat.hpp"

namespace test{
    void testZiggurat(){
        ziggurat zig;
        for (int i = 0; i < ziggurat::ZIGGURAT_N; ++i){
            std::cout << zig.next_normal() << " ";
        }
    }
}

