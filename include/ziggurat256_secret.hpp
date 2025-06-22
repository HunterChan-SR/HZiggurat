#include <cryptoTools/Common/block.h>
#include "ziggurat256.hpp"

class ziggurat256_secret{
    private:
    std::vector<osuCrypto::block> X;
    std::vector<osuCrypto::block> Y;

    public:
        ziggurat256_secret(const ziggurat256& zig){
            zig.X;
        }

};