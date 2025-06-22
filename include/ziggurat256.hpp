#include <cmath>
#include <cstdint>
#include <random>
#include <vector>
#include <iostream>

using _float = long double;

class ziggurat256
{
protected:
    static constexpr uint32_t ZIGGURAT_N = 256;
    static constexpr _float AREA = 0.00492867323399;
    static constexpr _float R = 3.6541528853610088;

    std::vector<_float> X;
    std::vector<_float> Y;

    std::mt19937 rng;
    std::uniform_int_distribution<uint32_t> dist_uint32;
    std::uniform_real_distribution<_float> dist_float;
    std::uniform_real_distribution<_float> dist_uniform;

    
    void init_tables()
    {
        X[0] = R;
        Y[0] = std::exp(-0.5 * R * R);

        for (uint32_t i = 1; i < ZIGGURAT_N; ++i)
        {
            X[i] = std::sqrt(-2.0 * std::log(Y[i - 1] + AREA / X[i - 1]));
            Y[i] = std::exp(-0.5 * X[i] * X[i]);
        }

        X[ZIGGURAT_N] = 0.0;
        Y[ZIGGURAT_N] = 1.0;
    }
    _float sample_tail()
    {
        _float x, y;
        do
        {
            x = -std::log(dist_uniform(rng)) / R;
            y = -std::log(dist_uniform(rng));
        } while (2 * y < x * x);
        return R + x;
    }

public:
    ziggurat256() : X(ZIGGURAT_N + 1), Y(ZIGGURAT_N + 1),
                    rng(std::random_device{}()),
                    dist_uint32(0, ZIGGURAT_N - 1),
                    dist_float(-1.0, 1.0),
                    dist_uniform(0.0, 1.0)
    {
        init_tables();
    }

    _float next_normal()
    {
        while (true)
        {
            uint32_t i = dist_uint32(rng);
            _float u = dist_float(rng);
            _float z = u * X[i];

            if (std::abs(z) < X[i + 1])
            {
                return z;
            }
            if (i == 0)
            {
                _float tail = sample_tail();
                return (dist_uniform(rng) < 0.5) ? tail : -tail;
            }
            else if (dist_uniform(rng) < std::exp(-0.5 * (z * z - X[i] * X[i])))
            {
                return z;
            }
        }
    }

    // #ifdef DEBUG
    void debug_print() const
    {
#ifndef DEBUG
        return;
#endif
        for (size_t i = 0; i <= ZIGGURAT_N; ++i)
        {
            if (sizeof(_float) == sizeof(double))
            {
                //  double
                printf("X[%zu] = %.10f, Y[%zu] = %.10f \n ", i, X[i], i, Y[i]);
            }
            else if (sizeof(_float) == sizeof(long double))
            {
                //  long double
                printf("X[%zu] = %.10Lf, Y[%zu] = %.10Lf\n", i, X[i], i, Y[i]);
            }
        }
    }
    void chceck() const
    {
#ifndef DEBUG
        return;
#endif
        for (size_t i = 1; i < ZIGGURAT_N; ++i)
        {
            _float area = X[i] * (Y[i + 1] - Y[i]);
            std::cout << "Area[" << i << "] = " << area << std::endl;
            if (std::abs(area - AREA) > 1e-6)
            {
                std::cerr << "Area mismatch at index " << i << ": " << area << " != " << AREA << std::endl;
                exit(1);
            }
        }
        std::cout << "All areas match!" << std::endl;
    }
    // #endif
};