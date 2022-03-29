#include "Entity/Entity.h"
#include "Entity/Component.h"
#include "Graphics_headers.h"

namespace Lily {

class DirectionalLight public Component {
public:
	DirectionalLight(Entity self) : Component(self) {}
	glm::vec3 direction;
private:
};

}