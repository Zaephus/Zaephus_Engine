
#pragma once

#include <string>

#include <Component.h>
#include <Quaternion.h>
#include <Vectors/Vector3.h>

struct Matrix4x4;

class Transform : public Component {
    public:
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;

        Transform* parent = nullptr;

        bool hasChanged = false;

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
        [[nodiscard]] Matrix4x4 directionMatrix() const;

    protected:
        void update() override;

    private:
        Vector3 lastPos;
        Quaternion lastRot;
        Vector3 lastScale;
};