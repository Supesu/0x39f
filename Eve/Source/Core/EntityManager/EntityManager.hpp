#pragma once

#include <Core/Config.hpp>

#include <array>
#include <cassert>
#include <queue>

namespace eve
{
    class EntityManager
    {
    public:
        EntityManager();
        Entity CreateEntity();
        void DestroyEntity(Entity entity);
        void SetSignature(Entity entity, Signature signature);
        Signature GetSignature(Entity entity);

    private:
        std::queue<Entity> mAvailableEntities{};
        std::array<Signature, MAX_ENTITIES> mSignatures{};
        uint32_t mLivingEntityCount{};
    };
}