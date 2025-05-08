
#include "RenderItem.h"

#include "Action.h"
#include "Renderer.h"

RenderItem::RenderItem() {
    Renderer::startRenderItemCall.bind<RenderItem, &RenderItem::internalInitialize>(this);
}

RenderItem::~RenderItem() {
    Renderer::startRenderItemCall.unbind<RenderItem, &RenderItem::internalInitialize>(this);
}

void RenderItem::internalInitialize() {
    Renderer::startRenderItemCall.unbind<RenderItem, &RenderItem::internalInitialize>(this);

    initialize();
}