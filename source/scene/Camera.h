
#pragma once

#include <Matrices/Matrix4x4.h>
#include <Vectors/Vector2.h>

#include "GameObject.h"

class RayCast3D;
class Color;
struct Vector3;

class Camera : public GameObject {
    public:
        enum ProjectionType {
            none,
            perspective,
            orthographic
        };
        ProjectionType projectionType = none;

        float fovY = 45.0f;
        float size = 0.0f;
        float aspect = 800.0f/600.0f;
        float near = 0.1f;
        float far = 100.0f;

        bool projectionChanged = false;

        Camera();

        Matrix4x4 projectionMatrix = Matrix4x4::identity();
        [[nodiscard]] Matrix4x4 viewMatrix() const;

        void setAsActive();
        void onWindowResized(int _w, int _h);

        [[nodiscard]] Vector3 screenToWorldPos(const Vector2& _screenPos) const;
        [[nodiscard]] Vector3 screenToWorldPos(const Vector2& _screenPos, float _depth) const;
        [[nodiscard]] RayCast3D screenToRay(const Vector2& _screenPos) const;
        [[nodiscard]] RayCast3D screenToRay(const Vector2& _screenPos, bool _oneShot) const;

        static Camera* activeCam;

        static Camera* createPerspectiveCamera(float _fovY, float _near, float _far);
        static Camera* createOrthographicCamera(float _sizeY, float _near, float _far);

    private:
        Vector2 camRotation;

        Matrix4x4 lastProjMatrix;

        ~Camera() override;
        void update() override;

        static Matrix4x4 perspectiveMatrix(float _fovY, float _aspect, float _near, float _far);
        static Matrix4x4 orthographicMatrix(float _sizeY, float _aspect, float _near, float _far);

};