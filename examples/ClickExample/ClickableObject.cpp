
#include "ClickableObject.h"

#include <Bounds.h>

#include "MeshRenderer.h"
#include "Shader.h"
#include "TimeUtils.h"

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
                renderer->setColor("material.color", oldColor);
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
            oldColor = renderer->shader->getColor("material.color");
            renderer->setColor("material.color", clickedColor);
        }
    }
}