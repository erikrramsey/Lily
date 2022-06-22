#pragma once
#include "include/Entity/Component.h"

namespace Lily {

class Light : public Component {
public:
    Light(Entity self) : Component(self) {}
};

}