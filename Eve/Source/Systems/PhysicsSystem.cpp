#include "PhysicsSystem.hpp"

#include <glm/gtx/string_cast.hpp>

#include <Components/Renderable.hpp>
#include <Components/Transform.hpp>

#include <Core/System.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Gui.hpp>

#include <iostream>

extern eve::Coordinator gCoordinator;

namespace eve
{

    float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
    {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    }

    bool isInsideOfTriangle(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
    {
        bool b1 = sign(pt, v1, v2) < 0.0f;
        bool b2 = sign(pt, v2, v3) < 0.0f;
        bool b3 = sign(pt, v3, v1) < 0.0f;

        bool pointWasInsideTriangle = ((b1 == b2) && (b2 == b3));

        return pointWasInsideTriangle;
    }

    bool pointInsideOfModel(glm::vec2 &pt, Entity &entity)
    {
        auto &renderable = gCoordinator.GetComponent<Renderable>(entity);
        auto &transform = gCoordinator.GetComponent<Transform>(entity);

        auto vertices = renderable.model->getVertices();

        bool colliding = false;
        for (auto i = vertices.begin(); i != vertices.end(); i += 3)
        {
            auto v1 = (*i).position * transform.mat2() + transform.translation;
            auto v2 = (*(i + 1)).position * transform.mat2() + transform.translation;
            auto v3 = (*(i + 2)).position * transform.mat2() + transform.translation;

            bool pointInside = isInsideOfTriangle(pt, v1, v2, v3);

            if (pointInside)
            {
                colliding = true;
            }
        }

        return colliding;
    }

    bool pointInsideOfModel(glm::vec2 &pt, Entity &entity, glm::vec2 translation)
    {
        auto &renderable = gCoordinator.GetComponent<Renderable>(entity);
        auto &transform = gCoordinator.GetComponent<Transform>(entity);

        auto vertices = renderable.model->getVertices();

        bool colliding = false;
        for (auto i = vertices.begin(); i != vertices.end(); i += 3)
        {
            auto v1 = (*i).position * transform.mat2() + translation;
            auto v2 = (*(i + 1)).position * transform.mat2() + translation;
            auto v3 = (*(i + 2)).position * transform.mat2() + translation;

            bool pointInside = isInsideOfTriangle(pt, v1, v2, v3);

            if (pointInside)
            {
                colliding = true;
            }
        }

        return colliding;
    }

    bool PhysicsSystem::calculateEntityCollision(Entity toTest, Entity target)
    {
        bool colliding = false;

        auto &toTestRenderable = gCoordinator.GetComponent<Renderable>(toTest);
        auto &toTestTransform = gCoordinator.GetComponent<Transform>(toTest);

        auto &targetRenderable = gCoordinator.GetComponent<Renderable>(target);
        auto &targetTranform = gCoordinator.GetComponent<Transform>(target);

        auto toTestVertices = targetRenderable.model->getVertices();
        auto objVertices = toTestRenderable.model->getVertices();

        for (auto i = toTestVertices.begin(); i != toTestVertices.end(); i += 1)
        {
            auto pt = (*i).position * targetTranform.mat2() + targetTranform.translation;

            bool c = pointInsideOfModel(pt, toTest);

            if (c)
                colliding = true;
        }

        for (auto i = objVertices.begin(); i != objVertices.end(); i += 1)
        {
            auto pt = (*i).position * toTestTransform.mat2() + toTestTransform.translation;

            bool c = pointInsideOfModel(pt, target);

            if (c)
                colliding = true;
        }

        return colliding;
    }

    bool PhysicsSystem::calculateEntityCollision(Entity toTest, Entity target, glm::vec2 toTestTranslation)
    {
        bool colliding = false;

        auto &toTestRenderable = gCoordinator.GetComponent<Renderable>(toTest);
        auto &toTestTransform = gCoordinator.GetComponent<Transform>(toTest);

        auto &targetRenderable = gCoordinator.GetComponent<Renderable>(target);
        auto &targetTranform = gCoordinator.GetComponent<Transform>(target);

        auto toTestVertices = targetRenderable.model->getVertices();
        auto objVertices = toTestRenderable.model->getVertices();

        for (auto i = toTestVertices.begin(); i != toTestVertices.end(); i += 1)
        {
            auto pt = (*i).position * targetTranform.mat2() + targetTranform.translation;

            bool c = pointInsideOfModel(pt, toTest, toTestTranslation);

            if (c)
                colliding = true;
        }

        for (auto i = objVertices.begin(); i != objVertices.end(); i += 1)
        {
            auto pt = (*i).position * toTestTransform.mat2() + toTestTranslation;

            bool c = pointInsideOfModel(pt, target);

            if (c)
                colliding = true;
        }

        return colliding;
    }

    void PhysicsSystem::Init(){};

    void PhysicsSystem::Update(float dt)
    {
        nextPoll += dt;
        if (nextPoll > 1.0 / pollRate)
        {
            activeCollisions.clear();

            nextPoll -= 1.0 / pollRate;
        }

        for (auto entity : mEntities)
        {
            for (auto target : mEntities)
            {
                if (entity == target)
                    continue;

                Collision collision{};
                collision.AA = entity;
                collision.BB = target;

                auto it = std::find_if(activeCollisions.begin(), activeCollisions.end(), [&collision](const eve::Collision &obj)
                                       { return (obj.AA == collision.AA && obj.BB == collision.BB) || (obj.BB == collision.AA && obj.AA == collision.BB); });

                if (it == activeCollisions.end())
                {
                    if (calculateEntityCollision(entity, target))
                    {
                        activeCollisions.push_back(collision);
                    }
                }
            }
        }
    };
}
