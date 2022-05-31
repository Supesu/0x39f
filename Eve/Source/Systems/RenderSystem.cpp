#include "RenderSystem.hpp"

#include <Components/Renderable.hpp>
#include <Components/Transform.hpp>
#include <Core/Device/Device.hpp>

#include <iostream>
#include <memory>

extern eve::Coordinator gCoordinator;

namespace eve
{
    struct PushConstantData
    {
        glm::mat2 transform{1.f};
        glm::vec2 offset{1.f};
        alignas(16) glm::vec3 color{};
    };

    void RenderSystem::Init(Device &device, VkRenderPass renderPass)
    {
        createPipelineLayout(device);
        createPipeline(device, renderPass);
    };

    void RenderSystem::Update(VkCommandBuffer commandBuffer, Renderer &renderer, float dt)
    {
        float aspect = renderer.getAspectRatio();
        pipeline->bind(commandBuffer);

        for (auto const &entity : mEntities)
        {
            auto &transform = gCoordinator.GetComponent<Transform>(entity);
            auto const &renderable = gCoordinator.GetComponent<Renderable>(entity);

            PushConstantData push{};
            push.color = renderable.color;
            push.transform = transform.mat2(); // TODO: rewrite projection matrix (kinda scuffed)
            push.offset = transform.translation;

            vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantData), &push);
            renderable.model->bind(commandBuffer);
            renderable.model->draw(commandBuffer);
        }
    };

    // Private
    void RenderSystem::createPipelineLayout(Device &device)
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout");
        }
    }

    void RenderSystem::createPipeline(Device &device, VkRenderPass renderPass)
    {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipelinelayout!");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::defaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<eve::Pipeline>(device,
                                                   "/Users/supesu/dev/code/0x39f/Build/Release/macOS/Shaders/shader.vert.spv",
                                                   "/Users/supesu/dev/code/0x39f/Build/Release/macOS/Shaders/shader.frag.spv",
                                                   pipelineConfig);
    }
}