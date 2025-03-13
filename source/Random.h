
#pragma once

#include <cstdlib>

#include <TimeUtils.h>

class Random {
    public:
        template <typename T>
        [[nodiscard]] static T range() {
            setSeed();
            return static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX);
        }

        template <typename T>
        [[nodiscard]] static T range(T _from, T _to) {
            setSeed();
            return _from + static_cast<T>(std::rand()) / (static_cast<T>(RAND_MAX) / (_to - _from));
        }

    private:
        inline static int counter = 1;

        static void setSeed() {
            std::srand(counter * Time::microseconds());
            counter++;
        }
};