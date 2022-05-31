#pragma once

#include <Core/System.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Gui.hpp>

namespace eve
{
    class GuiSystem : public System
    {
    public:
        void Init();
        void Update(VkCommandBuffer commandBuffer, float dt, Gui &gu);
    };
}