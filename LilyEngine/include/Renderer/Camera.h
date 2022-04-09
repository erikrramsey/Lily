#pragma once

#include "Renderer/Graphics_headers.h"
#include "Entity/Component.h"
#include "Entity/Entity.h"

namespace Lily {
class Camera : public Component {
public:
	Camera(Entity self);
	~Camera();
	bool Initialize(int w, int h);
	glm::mat4 Camera::GetProjection() { return Projection; }
    glm::mat4 Camera::GetView(){ return view; }

	void update();

	void pitch_in(float input);
	void yaw_in(float input);
	void RollIn(bool input);

	float Pitch;
	float Yaw;

	glm::vec3 Up;
	glm::vec3 WorldUp;
	glm::vec3 forward;
	glm::vec3 Strafe;
	glm::vec3 position;

	float MouseSensitivity;
	float move_speed;

private:
	glm::mat4 Projection;
	glm::mat4 view;
};

}
