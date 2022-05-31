#pragma once

#include <Core/System.hpp>
#include <Core/Coordinator.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace eve
{
    class CameraControlSystem : public System
    {
    public:
        void Init();
        void Update(float left, float right, float bottom, float top, float near, float far);

        glm::mat4 getProjection() { return projectionMatrix; }

    private:
        glm::mat4 projectionMatrix{1.f};
    };
}
