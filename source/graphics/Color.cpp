
#include "Color.h"

#include <format>

#include <ZMath.h>

Color::Color() {
    *this = white();
}

Color::Color(float _r, float _g, float _b, float _a) {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
}

Color::Color(const Vector4 &_v) {
    r = _v.x;
    g = _v.y;
    b = _v.z;
    a = _v.w;
}

std::string Color::toString() const {
    return std::format("{}, {}, {}, {}", r, g, b, a);
}

[[nodiscard]] float Color::magnitude() const {
    return std::sqrt(r*r + g*g + b*b + a*a);
}
[[nodiscard]] float Color::squaredMagnitude() const {
    return r*r + g*g + b*b + a*a;
}

[[nodiscard]] Color Color::normalized() const {
    Color norm = Color(r, g, b, a);
    norm.normalize();
    return norm;
}
void Color::normalize() {
    *this /= magnitude();
}

Color Color::red()     { return { 1.0f, 0.0f, 0.0f, 1.0f }; }
Color Color::green()   { return { 0.0f, 1.0f, 0.0f, 1.0f }; }
Color Color::blue()    { return { 0.0f, 0.0f, 1.0f, 1.0f }; }
Color Color::white()   { return { 1.0f, 1.0f, 1.0f, 1.0f }; }
Color Color::black()   { return { 0.0f, 0.0f, 0.0f, 1.0f }; }
Color Color::yellow()  { return { 1.0f, 0.921f, 0.016f, 1.0f }; }
Color Color::cyan()    { return { 0.0f, 1.0f, 1.0f, 1.0f }; }
Color Color::magenta() { return { 1.0f, 0.0f, 1.0f, 1.0f }; }
Color Color::grey()    { return { 0.5f, 0.5f, 0.5f, 1.0f }; }
Color Color::brown()   { return { 0.4f, 0.2f, 0.0f, 1.0f }; }

Color& Color::operator+=(const Color &_c) {
    *this = *this + _c;
    return *this;
}
Color operator+(const Color &_lhs, const Color &_rhs) {
    return {
        _lhs.r + _rhs.r,
        _lhs.g + _rhs.g,
        _lhs.b + _rhs.b,
        _lhs.a + _rhs.a
    };
}

Color& Color::operator-=(const Color &_c) {
    *this = *this + _c;
    return *this;
}
Color operator-(const Color &_lhs, const Color &_rhs) {
    return {
        _lhs.r - _rhs.r,
        _lhs.g - _rhs.g,
        _lhs.b - _rhs.b,
        _lhs.a - _rhs.a
    };
}
Color Color::operator-() {
    *this = -1 * *this;
    return *this;
}

Color& Color::operator*=(float _s) {
    *this = *this * _s;
    return *this;
}
Color operator*(const Color &_c, float _s) {
    return {
        _c.r * _s,
        _c.g * _s,
        _c.b * _s,
        _c.a * _s
    };
}
Color operator*(float _s, const Color &_c) {
    return _c * _s;
}

Color& Color::operator/=(float _s) {
    *this = *this / _s;
    return *this;
}
Color operator/(const Color &_c, float _s) {
    return {
        _c.r / _s,
        _c.g / _s,
        _c.b / _s,
        _c.a / _s
    };
}

bool operator==(const Color &_lhs, const Color &_rhs) {
    return _lhs.r == _rhs.r && _lhs.g == _rhs.g && _lhs.b == _rhs.b && _lhs.a == _rhs.a;
}
bool operator!=(const Color &_lhs, const Color &_rhs) {
    return !(_lhs == _rhs);
}

float Color::operator[](unsigned int _i) const {
    switch(_i) {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        case 3: return a;
        default: throw std::out_of_range("Invalid index.");
    }
}