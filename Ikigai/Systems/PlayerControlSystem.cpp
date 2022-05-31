#include "PlayerControlSystem.hpp"

#include <Core/System.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Gui.hpp>

#include <iostream>

#include <Systems/PhysicsSystem.hpp>

extern eve::Coordinator gCoordinator;

namespace eve
{
    bool PlayerControlSystem::keyIsPressed(GLFWwindow *window, int key)
    {
        return glfwGetKey(window, key) == GLFW_PRESS;
    };

    void PlayerControlSystem::Init(){};

    void PlayerControlSystem::Update(GLFWwindow *window, float dt)
    {
        auto playerIt = std::find_if(
            mEntities.begin(), mEntities.end(), [](const eve::Entity &entity)
            { 
                                        auto &player = gCoordinator.GetComponent<Player>(entity);
                                        return player.isAlive; });

        auto player = *playerIt;
        auto &transform = gCoordinator.GetComponent<Transform>(player);
        glm::vec2 translation{};

        if (keyIsPressed(window, keys.jump) || keyIsPressed(window, keys.alternative_jump))
        {
            translation.y -= (dt * jumpForce);
        }

        if (keyIsPressed(window, keys.stomp) || keyIsPressed(window, keys.alternative_stomp))
        {
            translation.y += (dt * jumpForce);
        }

        if (keyIsPressed(window, keys.left) || keyIsPressed(window, keys.alternative_left))
        {
            translation.x -= (dt * runSpeed);
        }

        if (keyIsPressed(window, keys.right) || keyIsPressed(window, keys.alternative_right))
        {
            translation.x += (dt * runSpeed);
        }

        bool hasCollidedOnX{false};
        bool hasCollidedOnY{false};
        for (auto &target : mEntities)
        {
            if (player == target)
            {
                continue;
            }

            glm::vec2 theoreticalPositionX{};
            glm::vec2 theoreticalPositionY{};
            theoreticalPositionX.x = transform.translation.x + translation.x;
            theoreticalPositionY.y = transform.translation.y;
            theoreticalPositionY.y = transform.translation.y + translation.y;
            theoreticalPositionY.x = transform.translation.x;

            if (!hasCollidedOnX)
            {
                bool isColliding{false};

                if (theoreticalPositionX.x >= 1.0f || theoreticalPositionX.x <= -1.0f)
                    hasCollidedOnX = true;
                else
                    isColliding = PhysicsSystem::calculateEntityCollision(player, target, theoreticalPositionX);

                if (isColliding)
                {
                    hasCollidedOnX = true;
                }
            }

            if (!hasCollidedOnY)
            {
                bool isColliding{false};

                if (theoreticalPositionY.y >= 1.0f || theoreticalPositionY.y <= -1.0f)
                    hasCollidedOnY = true;
                else
                    isColliding = PhysicsSystem::calculateEntityCollision(player, target, theoreticalPositionY);

                if (isColliding)
                {
                    hasCollidedOnY = true;
                }
            }
        }

        if (!hasCollidedOnX)
        {
            transform.translation.x += translation.x;
        }

        if (!hasCollidedOnY)
        {
            transform.translation.y += translation.y;
        }
    };

    void PlayerControlSystem::SetPosition(glm::vec2 translation)
    {
        auto playerIt = std::find_if(
            mEntities.begin(), mEntities.end(), [](const eve::Entity &entity)
            { 
                                        auto &player = gCoordinator.GetComponent<Player>(entity);
                                        return player.isAlive; });

        auto player = *playerIt;
        auto &transform = gCoordinator.GetComponent<Transform>(player);
        transform.translation = translation;
    }
}