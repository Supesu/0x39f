#include "GuiSystem.hpp"

#include <Core/System.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Gui.hpp>

extern eve::Coordinator gCoordinator;

namespace eve
{
    void GuiSystem::Init(){};

    void GuiSystem::Update(VkCommandBuffer commandBuffer, float dt, Gui &gui)
    {
        gui.newFrame();
        gui.buildGui(dt);
        gui.render(commandBuffer);
    };
}
