
#include "Component.h"

const std::type_info& Component::getType() {
    return typeid(*this);
}