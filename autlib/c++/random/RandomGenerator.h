
#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>
#include <chrono>

namespace AutLib {
    class RandomGenerator {
    public:
        RandomGenerator()
        {
            typedef std::chrono::high_resolution_clock clock;
            clock::time_point now = clock::now();
            auto seed = now.time_since_epoch().count();     //获取一个高精度的种子
            random_generator_.seed((unsigned int) seed);
        }

        int32_t RandomInt(int32_t low, int32_t high)
        {
            //returns integer in range [low, high]
            std::uniform_int_distribution<int32_t> distrib(low, high);  //使用均匀分布
            return distrib(random_generator_);
        }

        double RandomRange(double low, double high)
        {
            //returns double in range [low, high)
            std::uniform_real_distribution<double> distrib(low, high);
            return distrib(random_generator_);
        }

        double NextDouble()
        {
            return RandomRange (0, 1);
        }

        std::default_random_engine& GetRandomEngine()
        {
            return random_generator_;
        }

    private:
        std::default_random_engine random_generator_;
    };
}

#endif //RANDOMGENERATOR_H
