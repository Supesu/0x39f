#pragma once

#include <bitset>
#include <cstdint>
#include <vector>

namespace eve
{
    using Entity = std::uint32_t;
    const Entity MAX_ENTITIES = 5000;
    using ComponentType = std::uint8_t;
    const ComponentType MAX_COMPONENTS = 32;
    using Signature = std::bitset<MAX_COMPONENTS>;

    struct Collision
    {
        Entity AA;
        Entity BB;
    };

    struct Config
    {
        std::vector<Collision> collisions{};
        bool shouldHandleGravity{true};
        bool shouldResetPlayer{false};
        bool showColliderBoxes{false};
        bool shouldLogShaderData{false};
        bool shouldDisableCollisions{false};
        bool toggleLogStream{false};
    };
}