
#pragma once

#include <string>

#include <Quaternion.h>
#include <Vectors/Vector3.h>

struct Matrix4x4;

class Transform {
    public:
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;

        Transform();
        Transform(const Vector3 &_p, const Quaternion &_r, const Vector3 &_s);

        [[nodiscard]] std::string toString() const;

        void rotate(float _xDeg, float _yDeg, float _zDeg);
        void rotate(const Vector3& _eulerAngles);
        void rotate(const Quaternion& _q);

        void rotateAround(const Vector3 &_point, const Vector3& _axis, float _deg);
        void rotateAround(const Vector3 &_point, const Vector3& _axis, float _deg, bool _lookAtAxis);

        void lookAt(const Vector3& _point);
        void lookAt(const Vector3& _point, const Vector3& _up);

        [[nodiscard]] Vector3 right() const;
        [[nodiscard]] Vector3 forward() const;
        [[nodiscard]] Vector3 up() const;

        [[nodiscard]] Matrix4x4 objectMatrix() const;
};