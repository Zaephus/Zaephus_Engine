
#pragma once

#include <cstdlib>
#include <random>

#include <TimeUtils.h>

class Random {
    public:
        template <typename T>
        [[nodiscard]] static T range() {
            setSeed();
            return static_cast<T>(rng()) / static_cast<T>(rng.max);
        }

        template <typename T>
        [[nodiscard]] static T range(T _from, T _to) {
            if(_to - _from == 0) {
                return static_cast<T>(0);
            }
            //
            setSeed();
            // return _from + static_cast<T>(std::rand()) / (static_cast<T>(RAND_MAX) / (_to - _from));


            return _from + static_cast<T>(rng()) / (static_cast<T>(rng.max()) / (_to - _from));
        }

    private:
        inline static int counter = 1;
        inline static std::ranlux24_base rng = std::ranlux24_base();

        static void setSeed() {
            rng.seed(counter * Time::microseconds());
            counter++;
        }
};