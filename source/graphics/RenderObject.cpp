
#include "RenderObject.h"

#include "Action.h"
#include "Renderer.h"

RenderObject::RenderObject() {
    Renderer::initRenderObjectCall.bind<RenderObject, &RenderObject::internalInitialize>(this);
}

RenderObject::~RenderObject() {
    Renderer::initRenderObjectCall.unbind<RenderObject, &RenderObject::internalInitialize>(this);
}

void RenderObject::internalInitialize() {
    Renderer::initRenderObjectCall.unbind<RenderObject, &RenderObject::internalInitialize>(this);

    initialize();
}