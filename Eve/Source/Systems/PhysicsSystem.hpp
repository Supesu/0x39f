#pragma once

#include <set>

#include <Core/System.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Gui.hpp>

namespace eve
{
    class PhysicsSystem : public System
    {
    public:
        bool calculateEntityCollision(Entity toTest, Entity target);
        static bool calculateEntityCollision(Entity toTest, Entity target, glm::vec2 toTestTranslation);
        std::vector<Collision> getActiveCollisions() { return activeCollisions; };

        void Init();
        void Update(float dt);

    private:
        // store
        std::vector<Collision> activeCollisions{};

        // config
        float pollRate{4.0};
        float nextPoll{0.0};
    };
}