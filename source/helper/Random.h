
#pragma once

#include <cstdlib>
#include <random>

#include "TimeUtils.h"

class Random {
    public:
        template <typename T>
        [[nodiscard]] static T range() {
            setSeed();
            return rng() / max;
        }

        template <typename T>
        [[nodiscard]] static T range(T _from, T _to) {
            if(_to - _from == 0) {
                return static_cast<T>(0);
            }

            setSeed();
            return _from + rng() / (max / (_to - _from));
        }

    private:
        inline static int counter = 1;
        inline static std::mt19937 rng; // NOLINT(*-msc51-cpp)
        inline static unsigned int max = std::mt19937::max();

        static void setSeed() {
            rng.seed(counter * Time::microseconds());
            counter++;
        }
};