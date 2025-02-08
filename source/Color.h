
#pragma once

#include <string>

struct Vector4;

struct Color {

    float r = 0;
    float g = 0;
    float b = 0;
    float a = 0;

    Color();
    explicit Color(const float _x) : Color(_x, _x, _x, _x) {};
    Color(float _r, float _g, float _b, float _a);
    Color(const Vector4& _v); // NOLINT(*-explicit-constructor)

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] float magnitude() const;
    [[nodiscard]] float squaredMagnitude() const;

    [[nodiscard]] Color normalized() const;
    void normalize();

    static Color red();
    static Color green();
    static Color blue();
    static Color white();
    static Color black();
    static Color yellow();
    static Color cyan();
    static Color magenta();
    static Color grey();
    static Color brown();

    Color& operator+=(const Color& _v);
    friend Color operator+(const Color& _lhs, const Color& _rhs);

    Color& operator-=(const Color& _v);
    friend Color operator-(const Color& _lhs, const Color& _rhs);
    Color operator-();

    Color& operator*=(float _s);
    friend Color operator*(const Color& _v, float _s);
    friend Color operator*(float _s, const Color& _v);

    Color& operator/=(float _s);
    friend Color operator/(const Color& _v, float _s);

    Color& operator=(const Color& _v) = default;

    friend bool operator==(const Color& _lhs, const Color& _rhs);
    friend bool operator!=(const Color& _lhs, const Color& _rhs);

    float operator[](unsigned int _i) const;

};