
#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>
#include <ZMath.h>

#include "Camera.h"
#include "Color.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "Shader.h"
#include "TimeUtils.h"

class BoidsScene : public Scene {

    struct Boid {
        Vector3 position;
        Vector3 velocity;

        Boid(const Vector3& _pos, const Vector3& _vel) {
            position = _pos;
            velocity = _vel;
        }
    };

    float width = 4.5f;
    float height = 4.5f;
    float depth = 4.5f;

    int boidAmount = 150;

    std::vector<Boid> boids;

    float turnFactor = 0.005f;
    float visualRange = 1.0f;
    float protectedRange = 0.6f;
    float centeringFactor = 0.0008f;
    float avoidFactor = 0.08f;
    float matchingFactor = 0.005f;
    float maxSpeed = 6.0f;
    float minSpeed = 3.0f;

    MeshRenderer* boidModel = nullptr;
    MeshRenderer* boxModel = nullptr;
    Camera* cam = nullptr;

    Vector3 getRandomPosition(const float _w, const float _h, const float _d, const int _index) {
        std::srand(static_cast<unsigned int>(Time::nanoseconds() * _index));

        return {
            std::rand() / ((RAND_MAX + 1u) / _w) * 2 - _w,
            std::rand() / ((RAND_MAX + 1u) / _h) * 2 - _h,
            std::rand() / ((RAND_MAX + 1u) / _d) * 2 - _d
        };
    }

    Vector3 getRandomVelocity(const float _min, const float _max, const int _index) {
        std::srand(static_cast<unsigned int>(Time::nanoseconds() * _index));

        return {
            std::rand() / ((RAND_MAX + 1u) / (_max - _min)) * 2 - (_max - _min),
            std::rand() / ((RAND_MAX + 1u) / (_max - _min)) * 2 - (_max - _min),
            std::rand() / ((RAND_MAX + 1u) / (_max - _min)) * 2 - (_max - _min)
        };
    }

    public:
        void start() override {

            shouldRenderAxis = false;

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 12.0f / 6.0f, 0.1f, 100.0f);
            cam->transform->position = { 0.0f, 0.0f, 22.0f };
            cam->setClearColor(Color::black());

            Shader* boidShader = Shader::unlitShader(Color::green());

            boidModel = MeshRenderer::loadModel("resources/models/barbed_arrowhead.obj");
            boidModel->setOverrideShader(boidShader);
            boidModel->transform->position = Vector3::zero();
            boidModel->transform->scale = Vector3::one() * 4.0f;

            Shader* boxShader = Shader::unlitShader(0.1f, 0.5f, 0.5f, 0.3f);

            boxModel = MeshRenderer::loadModel(MeshRenderer::cube);
            boxModel->setOverrideShader(boxShader);
            boxModel->transform->position = Vector3::zero();
            boxModel->transform->scale = Vector3::one() * 12.0f;

            for(int i = 0; i < boidAmount; i++) {
                Boid boid(
                    getRandomPosition(width, height, depth, i),
                    getRandomVelocity(minSpeed, maxSpeed, i)
                );
                boids.push_back(boid);
            }
        }

        void update() override {
            for(int i = 0; i < boidAmount; i++) {

                Vector3 avoidAvgPos = Vector3::zero();
                Vector3 velAvg = Vector3::zero();
                Vector3 posAvg = Vector3::zero();

                int boidNeighbours = 0;

                for(int j = 0; j < boidAmount; j++) {

                    if(j == i) { continue; }

                    const float dist = Vector3::distance(boids[i].position, boids[j].position);

                    if(dist < protectedRange) {
                        avoidAvgPos += boids[i].position - boids[j].position;
                        continue;
                    }

                    if(dist < visualRange) {
                        velAvg += boids[j].velocity;
                        posAvg += boids[j].position;
                        boidNeighbours ++;
                    }
                }

                if(boidNeighbours > 0) {
                    velAvg /= static_cast<float>(boidNeighbours);
                    posAvg /= static_cast<float>(boidNeighbours);

                    boids[i].velocity += (velAvg - boids[i].velocity) * matchingFactor;
                    boids[i].velocity += (posAvg - boids[i].position) * centeringFactor;
                }

                boids[i].velocity += avoidAvgPos * avoidFactor;

                if(boids[i].position.x < -width) {
                    boids[i].velocity.x += turnFactor;
                }
                if(boids[i].position.x > width) {
                    boids[i].velocity.x -= turnFactor;
                }
                if(boids[i].position.y < -height) {
                    boids[i].velocity.y += turnFactor;
                }
                if(boids[i].position.y > height) {
                    boids[i].velocity.y -= turnFactor;
                }
                if(boids[i].position.z < -depth) {
                    boids[i].velocity.z += turnFactor;
                }
                if(boids[i].position.z > depth) {
                    boids[i].velocity.z -= turnFactor;
                }

                if(boids[i].velocity.magnitude() > maxSpeed) {
                    boids[i].velocity = boids[i].velocity.normalized() * maxSpeed;
                }
                if(boids[i].velocity.magnitude() < minSpeed) {
                    boids[i].velocity = boids[i].velocity.normalized() * minSpeed;
                }

                boids[i].position += boids[i].velocity * Time::deltaTime;
                boidModel->transform->position = boids[i].position;

                const float angle = Vector3::angle(Vector3::up(), boids[i].velocity.normalized());
                const Vector3 upAxis = Vector3::cross(Vector3::up(), boids[i].velocity.normalized());

                boidModel->transform->rotation = Quaternion::fromAxisAngle(upAxis, angle);
                boidModel->render();
            }

//            boxModel->transform->rotate(0.0f, 5.0f * Time::deltaTime, 5.0f * Time::deltaTime);
            boxModel->render();
        }

        ~BoidsScene() override {
            delete boidModel;
            delete boxModel;

            delete cam;
        }
};

int main() {
    BoidsScene scene;
    scene.initialize();
}