
#include "ClickableObject.h"

#include <ZEngine.h>

ClickableObject::ClickableObject() {
    addComponent(new Bounds());
}


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
                renderer->getShader()->setColor("material.color", oldColor);
            }
        }
    }
}

void ClickableObject::clicked() {
    if(!isClicked) {
        isClicked = true;
        timer = maxTime;

        const MeshRenderer* renderer = getComponent<MeshRenderer>();
        if(renderer != nullptr) {
            oldColor = renderer->getShader()->getColor("material.color");
            renderer->getShader()->setColor("material.color", clickedColor);
        }
    }
}