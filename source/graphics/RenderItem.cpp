
#include "RenderItem.h"

#include "Action.h"
#include "Renderer.h"

RenderItem::RenderItem() {
    Renderer::initRenderItemCall.bind<RenderItem, &RenderItem::internalInitialize>(this);
    Renderer::destroyRenderItemCall.bind<RenderItem, &RenderItem::internalDestroy>(this);
}

RenderItem::~RenderItem() {
    Renderer::initRenderItemCall.unbind<RenderItem, &RenderItem::internalInitialize>(this);
    Renderer::destroyRenderItemCall.unbind<RenderItem, &RenderItem::internalDestroy>(this);
}

void RenderItem::internalInitialize() {
    Renderer::initRenderItemCall.unbind<RenderItem, &RenderItem::internalInitialize>(this);

    initialize();
}

void RenderItem::internalDestroy() {
    Renderer::destroyRenderItemCall.unbind<RenderItem, &RenderItem::internalDestroy>(this);

    destroy();
}