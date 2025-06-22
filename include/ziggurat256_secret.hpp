#include <cryptoTools/Common/block.h>
#include "ziggurat256.hpp"

using _float = long double;

class ziggurat256_secret : public ziggurat256
{
private:
    std::vector<osuCrypto::block> sX;
    std::vector<osuCrypto::block> sY;

    static osuCrypto::block &_Float2block(const _float &value)
    {
        // 未实现真实逻辑，暂时先这样，以后改！！
        // __m128i mdata;
        // std::memcpy(&mdata, &value, sizeof(_float));
        // block = block(mdata);
    }
    static _float &block2_Float(const osuCrypto::block &block)
    {
        // 未实现真实逻辑，暂时先这样，以后改！！
    }

public:
    ziggurat256_secret(const ziggurat256 &zig):ziggurat256()
    {
        for(size_t i=0;i<=ZIGGURAT_N;i++){
            sX.push_back(_Float2block(X[i]));
            sY.push_back(_Float2block(Y[i]));
        }
    }
};