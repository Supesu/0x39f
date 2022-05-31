#pragma once

#include <Core/Device/Device.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Renderer/Renderer.hpp>
#include <Core/Window.hpp>
#include <Core/Gui.hpp>
#include <Core/Model/Model.hpp>

#include <memory>
#include <vector>

namespace ikigai
{
    class App
    {
    public:
        static constexpr int WIDTH = 1440;
        static constexpr int HEIGHT = 900;

        App();
        ~App();

        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void run();

    private:
        std::unique_ptr<eve::Model> createSquareModel(eve::Device &device, glm::vec2 offset);
        
        eve::Window window{WIDTH, HEIGHT, "Ikigai"};
        eve::Device device{window};
        eve::Renderer renderer{window, device};

        eve::Config config{};
        eve::Gui gui{
            window,
            device,
            renderer.getSwapChainRenderPass(),
            renderer.getImageCount(),
            config};
    };
}