
#include "ClickableObject.h"

#include <ZEngine.h>

ClickableObject::ClickableObject() {
    addComponent(new Bounds());
}

void ClickableObject::start() {
    shader = getComponent<MeshRenderer>()->getShader();
}

void ClickableObject::update() {
    if(isClicked) {
        if(timer > 0.0f) {
            timer -= Time::deltaTime;
        }
        else {
            timer = 0.0f;
            isClicked = false;

            shader->setColor("material.color", oldColor);
        }
    }
}

void ClickableObject::clicked() {
    if(!isClicked) {
        isClicked = true;
        timer = maxTime;

        oldColor = shader->getColor("material.color");
        shader->setColor("material.color", clickedColor);
    }
}