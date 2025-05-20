
#include "RenderItem.h"

#include "Action.h"
#include "Renderer.h"

RenderItem::RenderItem() {
    Renderer::initRenderItemCall.bind<RenderItem, &RenderItem::internalInitialize>(this);
}

RenderItem::~RenderItem() {
    Renderer::initRenderItemCall.unbind<RenderItem, &RenderItem::internalInitialize>(this);
}

void RenderItem::internalInitialize() {
    Renderer::initRenderItemCall.unbind<RenderItem, &RenderItem::internalInitialize>(this);

    initialize();
}