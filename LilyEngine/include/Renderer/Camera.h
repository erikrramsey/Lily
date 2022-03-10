#pragma once

#include "Renderer/Graphics_headers.h"

namespace Lily {

    enum Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    class Camera
    {
    public:
        Camera();
        ~Camera();
        bool Initialize(int w, int h);
        glm::mat4 GetProjection();
        glm::mat4 GetView();

        void Update();

        void PitchIn(float input);
        void YawIn(float input);
        void RollIn(bool input);

        void Translate(Direction direction, long long dt);

        float Pitch;
        float Yaw;

        glm::vec3 Up;
        glm::vec3 WorldUp;
        glm::vec3 Forward;
        glm::vec3 Strafe;

        glm::vec3 Position;

        float MouseSensitivity;

    private:
        float MoveSpeed;
        glm::mat4 Projection;
        glm::mat4 view;
    };

}
