
#include "ClickableObject.h"

#include <typeinfo>

#include "MeshRenderer.h"
#include "Shader.h"
#include "TimeUtils.h"

void ClickableObject::update() {
    if(isClicked) {
        if(timer > 0.0f) {
            timer -= Time::deltaTime;
        }
        else {
            timer = 0.0f;
            isClicked = false;

            const MeshRenderer* renderer = getComponent<MeshRenderer>();
            if(renderer != nullptr) {
                renderer->setColor("material.color", oldColor);
            }
        }
    }
}

void ClickableObject::clicked(int _button) {
    if(!isClicked) {
        isClicked = true;
        timer = maxTime;

        const MeshRenderer* renderer = getComponent<MeshRenderer>();

        std::cout << renderer << std::endl;
        if(renderer != nullptr) {
            oldColor = renderer->overrideShader->getColor("material.color");
            renderer->setColor("material.color", clickedColor);
        }
    }
}