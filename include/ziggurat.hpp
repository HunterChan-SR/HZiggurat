#pragma once
#include <cmath>
#include <cstdint>
#include <random>

class ziggurat
{
public:
    static constexpr int ZIGGURAT_N = 256;
    static constexpr uint32_t MAX_UINT32 = 0xFFFFFFFF;
private:
    uint32_t kn[ZIGGURAT_N];
    float wn[ZIGGURAT_N];
    float fn[ZIGGURAT_N];
    std::mt19937 rng;
    std::uniform_int_distribution<uint32_t> dist32;
    std::uniform_real_distribution<float> distf;

    void init_tables();

public:
    ziggurat(int seed = std::random_device{}());
    ~ziggurat() = default;

    float next_normal();
};


void ziggurat::init_tables()
{
    const double m1 = 2147483648.0; // 2^31
    const double vn = 9.91256303526217e-3;

    double dn = 3.442619855899;
    double tn = dn;
    double q = vn / std::exp(-0.5 * dn * dn);

    kn[0] = static_cast<uint32_t>((dn / q) * m1);
    kn[1] = 0;

    wn[0] = static_cast<float>(q / m1);
    wn[ZIGGURAT_N - 1] = static_cast<float>(dn / m1);

    fn[0] = 1.0f;
    fn[ZIGGURAT_N - 1] = static_cast<float>(std::exp(-0.5 * dn * dn));

    for (int i = ZIGGURAT_N - 2; i >= 1; --i)
    {
        dn = std::sqrt(-2.0 * std::log(vn / dn + std::exp(-0.5 * dn * dn)));
        kn[i + 1] = static_cast<uint32_t>((dn / tn) * m1);
        tn = dn;
        fn[i] = static_cast<float>(std::exp(-0.5 * dn * dn));
        wn[i] = static_cast<float>(dn / m1);
    }
}

ziggurat::ziggurat(int seed) : rng(seed), dist32(0, MAX_UINT32), distf(0.0f, 1.0f)
{
    init_tables();
}

float ziggurat::next_normal()
{
    for (;;)
    {
        uint32_t u = dist32(rng);
        int i = u & 0x7F;
        int sign = (u & 0x80) ? 1 : -1;
        uint32_t j = u >> 8;

        if (j < kn[i])
        {
            return sign * j * wn[i];
        }
        else
        {
            float x, y;
            if (i == 0)
            {
                // Tail sampling
                do {
                    x = -std::log(distf(rng)) / 3.442619855899;
                    y = -std::log(distf(rng));
                } while (y + y < x * x);
                return sign * (3.442619855899 + x);
            }
            else
            {
                x = j * wn[i];
                if (fn[i] + distf(rng) * (fn[i - 1] - fn[i]) < std::exp(-0.5f * x * x))
                    return sign * x;
            }
        }
    }
}

