
#include "ClickableObject.h"

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
        oldColor = renderer->overrideShader->getColor("material.color");
        if(renderer != nullptr) {
            renderer->setColor("material.color", clickedColor);
        }
    }
}