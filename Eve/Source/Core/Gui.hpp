
#pragma once

#include <Core/Device/Device.hpp>
#include <Core/Config.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <stdexcept>

namespace eve
{
    struct Fonts
    {
        ImFont *rudaBold12;
        ImFont *rudaBold18;
        ImFont *rudaBold16;
    };

    static void check_vk_result(VkResult err)
    {
        if (err == 0)
            return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if (err < 0)
            abort();
    }

    class Gui
    {
    public:
        Gui(Window &window, Device &device, VkRenderPass renderPass, uint32_t imageCount, Config &config);
        ~Gui();

        void newFrame();

        void render(VkCommandBuffer commandBuffer);

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        void buildGui(float &dt);
        void buildLogStream();

    private:
        Device &device;
        Fonts fonts{};
        Config &config;

        float mainWindowXSize;
        float mainWindowYPos;

        VkDescriptorPool descriptorPool;
    };
}