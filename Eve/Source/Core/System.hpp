#pragma once

#include <Core/Config.hpp>

#include <set>

namespace eve
{
    class System
    {
    public:
        std::set<Entity> mEntities;
    };
}