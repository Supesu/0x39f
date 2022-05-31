#pragma once

#include "../Device/Device.hpp"
#include "../Swapchain/Swapchain.hpp"
#include "../Window.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace eve
{
    class Renderer
    {
    public:
        Renderer(Window &window, Device &device);
        ~Renderer();

        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;

        float getAspectRatio() const { return swapchain->extentAspectRatio(); }
        bool isFrameInProgress() const { return isFrameStarted; }
        VkRenderPass getSwapChainRenderPass() const { return swapchain->getRenderPass(); }
        uint32_t getImageCount() const { return swapchain->imageCount(); }

        VkCommandBuffer getCurrentCommandBuffer() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const
        {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        Window &window;
        Device &device;
        std::unique_ptr<eve::SwapChain> swapchain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        bool isFrameStarted{false};
        int currentFrameIndex{0};
    };
}