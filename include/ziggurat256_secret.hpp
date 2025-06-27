#pragma once
#include <wmmintrin.h>
#include <cryptoTools/Common/block.h>
#include "ziggurat256.hpp"
#include <array>
using _float = long double;

_float Multiple = 1e6;

static osuCrypto::block _Float2block(const _float &value)
{
    __int128_t x = value * Multiple;
    uint64_t high = static_cast<uint64_t>(x >> 64);
    uint64_t low = static_cast<uint64_t>(x & 0xFFFFFFFFFFFFFFFF);
    osuCrypto::block xBlock(high, low);
    return xBlock;
}
static _float block2_Float(const osuCrypto::block &block)
{
    auto arr = block.get<uint64_t>();
    __int128_t x = 0;
    for (auto i : arr)
    {
        x = x << sizeof(uint64_t) * 8 | i;
    }
    _float value = static_cast<_float>(x) / Multiple;
    return value;
}

inline std::ostream& operator<<(std::ostream& os, const osuCrypto::block& b)
{
    auto arr = b.get<uint64_t>();
    os << "block(0x" << std::hex << arr[0] << ", 0x" << arr[1] << ")" << std::dec;
    return os;
}

class ziggurat256_secret : public ziggurat256
{
private:
    std::vector<osuCrypto::block> sX;
    std::vector<osuCrypto::block> sY;

public:
    ziggurat256_secret() : ziggurat256()
    {
        for (size_t i = 0; i <= ZIGGURAT_N; i++)
        {
            sX.push_back(_Float2block(X[i]));
            sY.push_back(_Float2block(Y[i]));
        }
    }

    void debug_block() const
    {
#ifndef DEBUG
        return;
#endif
        std::cout<<"\ndebug_block\n";
        for (size_t i = 0; i <= ZIGGURAT_N; ++i)
        {
            std::cout<<"sX[" << i << "] = " << sX[i] << ", sY[" << i << "] = " << sY[i] << std::endl;
        }
    }

};