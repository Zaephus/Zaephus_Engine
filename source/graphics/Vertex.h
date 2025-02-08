
#pragma once

#include <format>
#include <string>

#include <ZMath.h>
#include "../Color.h"

struct Vertex {
    Vector3 position;
    Color color;
    Vector2 uv;
    Vector3 normal;

    [[nodiscard]] std::string toString() const {
        return std::format("{}\n{},\n{},\n{}", position.toString(), color.toString(), uv.toString(), normal.toString());
    }
};