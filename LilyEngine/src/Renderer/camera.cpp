#include <glm/gtc/quaternion.hpp>

#include "Renderer/Camera.h"


namespace Lily {

    Camera::Camera(Entity self) : Component(self) {
        MouseSensitivity = 0;
        move_speed = 0;

        Yaw = 0;
        Pitch = 0;

        Up = glm::vec3(0.0f);
        forward = glm::vec3(0.0f);
        Strafe = glm::vec3(0.0f);
        position = glm::vec3(0.0f);

        WorldUp = glm::vec3(0.0f);

        Projection = glm::mat4(0.0f);
        view = glm::mat4(0.0f);
    }

    Camera::~Camera()
    {

    }

    bool Camera::Initialize(int w, int h)
    {
        Yaw = -90.0f;
        Pitch = 0.0f;

        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        WorldUp = Up;
        forward = glm::vec3(0.0f, 0.0f, -1.0f);
        Strafe = glm::cross(Up, forward);

        move_speed = 30;
        MouseSensitivity = 0.1f;

        position = glm::vec3(0, 0, 5);

        update();

        Projection = glm::perspective(45.0f, float(w) / float(h), 0.01f, 10000.0f);
        return true;
    }

    void Camera::update() {
        glm::vec3 front{
            cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),
            sin(glm::radians(Pitch)),
            sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))
        };

        forward = glm::normalize(front);

        Strafe = glm::normalize(glm::cross(forward, WorldUp));
        Up = glm::normalize(glm::cross(Strafe, forward));

        view = glm::lookAt(position, position + forward, Up);
    };

    void Camera::pitch_in(int input) {
        Pitch -= input * MouseSensitivity;
        if (Pitch > (89.0f)) Pitch = (89.0f);
        if (Pitch < (-89.0f)) Pitch = (-89.0f);
        update();
    }

    void Camera::yaw_in(int input) {
        Yaw += input * MouseSensitivity;
        update();
    }
}
