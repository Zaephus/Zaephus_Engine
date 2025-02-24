
#include "Component.h"

#include <typeinfo>

const std::type_info& Component::getType() {
    return typeid(*this);
}