#pragma once

#include <Components/Transform.hpp>
#include <Components/Player.hpp>
#include <Core/System.hpp>

#include <Systems/PhysicsSystem.hpp>

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eve
{
    class PlayerControlSystem : public System
    {
    public:
        void Init();
        void Update(GLFWwindow *window, float dt);
        void SetPosition(glm::vec2 translation);

    private:
        struct KeyMappings
        {
            int left = GLFW_KEY_A;
            int right = GLFW_KEY_D;
            int jump = GLFW_KEY_SPACE;
            int stomp = GLFW_KEY_S;

            int alternative_jump = GLFW_KEY_W;
            int alternative_left = GLFW_KEY_LEFT;
            int alternative_right = GLFW_KEY_RIGHT;
            int alternative_stomp = GLFW_KEY_DOWN;
        };

        bool keyIsPressed(GLFWwindow *window, int key);
        KeyMappings keys{};

        float runSpeed{0.8f};
        float jumpForce{1.6f};
    };
}