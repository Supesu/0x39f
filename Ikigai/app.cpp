#include "app.hpp"

#include "./Systems/PlayerControlSystem.hpp"

#include <Systems/RenderSystem.hpp>
#include <Systems/GuiSystem.hpp>
#include <Systems/PhysicsSystem.hpp>
#include <Systems/CameraControlSystem.hpp>

#include <Components/Player.hpp>
#include <Components/Renderable.hpp>
#include <Components/RigidBody.hpp>
#include <Components/Transform.hpp>

#include <Core/Gui.hpp>
#include <Core/Config.hpp>
#include <Core/Coordinator.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <time.h>

eve::Coordinator gCoordinator;

namespace ikigai
{
    void rgb_color_code(int rgb[])
    {
        int i;
        for (i = 0; i < 3; i++)
        {
            rgb[i] = rand() * 2032 % 256;
        }
    }

    App::App() {}

    App::~App()
    {
        glfwTerminate();
    }

    std::unique_ptr<eve::Model> App::createSquareModel(eve::Device &device, glm::vec2 offset)
    {
        std::vector<eve::Model::Vertex> vertices = {
            {{-0.5f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}},
            {{-0.5f, -0.5f}},
            {{0.5f, -0.5f}},
            {{0.5f, 0.5f}},
        };

        return std::make_unique<eve::Model>(device, vertices);
    }

    void App::run()
    {
        srand(time(0));

        std::shared_ptr<eve::Model> squareModel = createSquareModel(
            device,
            {.5f, .0f});

        gCoordinator.Init();

        gCoordinator.RegisterComponent<Renderable>();
        gCoordinator.RegisterComponent<Transform>();

        auto renderSystem = gCoordinator.RegisterSystem<eve::RenderSystem>();
        {
            eve::Signature signature;
            signature.set(gCoordinator.GetComponentType<Renderable>());
            signature.set(gCoordinator.GetComponentType<Transform>());
            gCoordinator.SetSystemSignature<eve::RenderSystem>(signature);
        }
        renderSystem->Init(device, renderer.getSwapChainRenderPass());

        auto guiSystem = gCoordinator.RegisterSystem<eve::GuiSystem>();
        {
            eve::Signature signature;
            gCoordinator.SetSystemSignature<eve::GuiSystem>(signature);
        }
        guiSystem->Init();

        int gridCount = 3;
        std::vector<eve::Entity> entities(gridCount * gridCount);
        for (int i = 0; i < gridCount; i++)
        {
            for (int j = 0; j < gridCount; j++)
            {
                int rgb[3];
                rgb_color_code(rgb);

                auto tile = gCoordinator.CreateEntity();

                gCoordinator.AddComponent(tile, Transform{
                                                    .translation = {
                                                        -0.15f + i * 0.15f,
                                                        -0.15f + j * 0.15f},
                                                    .scale = glm::vec2{0.15f}});
                gCoordinator.AddComponent(tile, Renderable{
                                                    .color = {rgb[0], rgb[1], rgb[2]},
                                                    .model = squareModel});

                entities.push_back(tile);
            }
        }

        float dt = 0.0;
        while (!window.shouldClose())
        {
            auto startTime = std::chrono::high_resolution_clock::now();
            glfwPollEvents();

            if (auto commandBuffer = renderer.beginFrame())
            {
                renderer.beginSwapChainRenderPass(commandBuffer);

                renderSystem->Update(commandBuffer, renderer, dt);
                guiSystem->Update(commandBuffer, dt, gui);

                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }

            auto stopTime = std::chrono::high_resolution_clock::now();
            dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        }

        vkDeviceWaitIdle(device.device());
    };
}