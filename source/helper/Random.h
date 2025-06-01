
#pragma once

#include <cstdlib>
#include <random>

#include "TimeUtils.h"

class Random {
    public:
        template <typename T>
        [[nodiscard]] static T range() {
            setSeed();
            return rng() / rng.max();
        }

        template <typename T>
        [[nodiscard]] static T range(T _from, T _to) {
            if(_to - _from == 0) {
                return static_cast<T>(0);
            }

            setSeed();
            return _from + rng() / (rng.max() / (_to - _from));
        }

    private:
        inline static int counter = 1;
        inline static std::mt19937 rng = std::mt19937();

        static void setSeed() {
            rng.seed(counter * Time::microseconds());
            counter++;
        }
};