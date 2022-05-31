#pragma once

#include <Core/Device/Device.hpp>
#include <Core/Pipeline/Pipeline.hpp>
#include <Core/System.hpp>
#include <Core/Renderer/Renderer.hpp>
#include <Core/Coordinator.hpp>
#include "CameraControlSystem.hpp"

#include <memory>
#include <vector>

namespace eve
{
    class RenderSystem : public System
    {
    public:
        void Init(Device &device, VkRenderPass renderPass);
        void Update(VkCommandBuffer commandBuffer, Renderer &renderer, float dt);

    private:
        void createPipelineLayout(Device &device);
        void createPipeline(Device &device, VkRenderPass renderPass);

        std::unique_ptr<Pipeline> pipeline;

        VkPipelineLayout pipelineLayout;
    };
}