#include "Gui.hpp"

#include "./Device/Device.hpp"
#include "./Window.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <stdexcept>

namespace eve
{

    Gui::Gui(
        Window &window, Device &device, VkRenderPass renderPass, uint32_t imageCount, Config &config)
        : device{device}, config{config}
    {
        VkDescriptorPoolSize pool_sizes[] = {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        if (vkCreateDescriptorPool(device.device(), &pool_info, nullptr, &descriptorPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to set up descriptor pool for imgui");
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;

        ImGui_ImplGlfw_InitForVulkan(window.getGLFWWindow(), true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = device.getInstance();
        init_info.PhysicalDevice = device.getPhysicalDevice();
        init_info.Device = device.device();
        init_info.QueueFamily = device.getGraphicsQueueFamily();
        init_info.Queue = device.graphicsQueue();

        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = descriptorPool;
        // todo, I should probably get around to integrating a memory allocator

        init_info.Allocator = VK_NULL_HANDLE;
        init_info.MinImageCount = 2;
        init_info.ImageCount = imageCount;
        init_info.CheckVkResultFn = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info, renderPass);

        fonts.rudaBold12 = io.Fonts->AddFontFromFileTTF("/Users/supesu/dev/code/0x39f/Build/Release/macOS/Fonts/Ruda-Bold.ttf", 12);
        fonts.rudaBold16 = io.Fonts->AddFontFromFileTTF("/Users/supesu/dev/code/0x39f/Build/Release/macOS/Fonts/Ruda-Bold.ttf", 16);
        fonts.rudaBold18 = io.Fonts->AddFontFromFileTTF("/Users/supesu/dev/code/0x39f/Build/Release/macOS/Fonts/Ruda-Bold.ttf", 18);

        auto commandBuffer = device.beginSingleTimeCommands();
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        device.endSingleTimeCommands(commandBuffer);
        ImGui_ImplVulkan_DestroyFontUploadObjects();

        ImGuiStyle *style = &ImGui::GetStyle();
        style->WindowPadding = ImVec2(15, 15);
        style->WindowRounding = 2.f;
        style->FramePadding = ImVec2(5, 5);
        style->FrameRounding = 3.0f;
        style->ItemSpacing = ImVec2(12, 8);
        style->ItemInnerSpacing = ImVec2(8, 6);
        style->IndentSpacing = 25.0f;
        style->ScrollbarSize = 15.0f;
        style->ScrollbarRounding = 9.0f;
        style->GrabMinSize = 5.0f;
        style->GrabRounding = 3.0f;
        style->WindowBorderSize = 0.0f;

        style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
        style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_WindowBg] = ImVec4(0.0062745098f, 0.005098039215f, 0.007450980392f, 1.00f);
        style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
        style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
        style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
        style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_Header] = ImVec4(0.010f, 0.009f, 0.012f, 1.00f);
        style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.056f, 0.056f, 0.058f, 1.00f);
        style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    }

    Gui::~Gui()
    {
        vkDestroyDescriptorPool(device.device(), descriptorPool, nullptr);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Gui::newFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Gui::render(VkCommandBuffer commandBuffer)
    {
        ImGui::Render();
        ImDrawData *drawdata = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawdata, commandBuffer);
    }

    void Gui::buildLogStream()
    {
        ImGui::Begin("Eve Engine Log Stream", (bool *)__null, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowPos(ImVec2(mainWindowXSize + 15.0f, mainWindowYPos));

        ImVec4 textColor = ImVec4(0.447058824f, 0.443137255f, 0.462745098f, 1.0f);
        ImVec4 accentColor = ImVec4(0.811764706f, 0.4f, 0.470588235f, 1.0f);

        ImGui::PushFont(fonts.rudaBold16);
        ImGui::TextColored(accentColor, "Log");
        ImGui::SameLine();
        ImVec2 cursor = ImGui::GetCursorPos();
        cursor.x -= 7.0f;
        ImGui::SetCursorPos(cursor);

        ImGui::TextColored(textColor, "Stream");
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();

        draw_list->AddRectFilled(ImVec2(p.x - 4.0f, p.y), ImVec2(p.x + 404.0f, p.y + 169.0f), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0122745098f, 0.008098039215f, 0.012450980392f, 1.00f)), 1.5f);
        ImGui::Dummy(ImVec2(400, 166));
        ImGui::PopFont();

        ImGui::End();
    }

    void Gui::buildGui(float &dt)
    {
        ImVec4 textColor = ImVec4(0.447058824f, 0.443137255f, 0.462745098f, 1.0f);
        ImVec4 accentColor = ImVec4(0.811764706f, 0.4f, 0.470588235f, 1.0f);

        ImGuiIO &io = ImGui::GetIO();
        ImGui::Begin("Eve Engine", (bool *)__null, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        ImGui::PushFont(fonts.rudaBold18);
        ImGui::TextColored(accentColor, "Eve");
        ImGui::SameLine();
        ImGui::TextColored(textColor, "v1.03");
        ImGui::PopFont();
        ImGui::Text("\"that wasn't meant to happen\"");

        ImVec2 cursor = ImGui::GetCursorPos();
        cursor.x += 250.0f;
        ImGui::SetCursorPos(cursor);
        ImGui::InvisibleButton("###PADDING", {1, 1});

        cursor = ImGui::GetCursorPos();
        cursor.y -= 3.0f;
        ImGui::SetCursorPos(cursor);
        if (ImGui::CollapsingHeader("Input Stream"))
        {
            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            ImGui::Text("Keys down:");
            for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
                if (ImGui::IsKeyDown(i))
                {
                    ImGui::SameLine();
                    ImGui::TextColored(textColor, "%d (0x%X) (%.1f secs)", i, i, io.KeysDownDuration[i]);
                }

            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            ImGui::Text("Keys pressed:");
            for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
                if (ImGui::IsKeyPressed(i))
                {
                    ImGui::SameLine();
                    ImGui::TextColored(textColor, "%d (0x%X)", i, i);
                }

            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            ImGui::Text("Keys release:");
            for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
                if (ImGui::IsKeyReleased(i))
                {
                    ImGui::SameLine();
                    ImGui::TextColored(textColor, "%d (0x%X)", i, i);
                }
        }

        if (ImGui::CollapsingHeader("Collisions Engine"))
        {
            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            ImGui::Text("Active Collisions:");

            for (auto &collision : config.collisions)
            {
                ImGui::SameLine();
                ImGui::TextColored(textColor, "%i : %i", collision.AA, collision.BB);
            }
        }

        if (ImGui::CollapsingHeader("Tools"))
        {
            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);

            if (ImGui::Button("Reset Player Position", ImVec2(ImGui::GetWindowSize().x * 0.85f, 0.0f)))
            {
                config.shouldResetPlayer = true;
            };

            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            if (ImGui::Button("Log Shader Data", ImVec2(ImGui::GetWindowSize().x * 0.85f, 0.0f)))
            {
                config.shouldLogShaderData = !config.shouldLogShaderData;
            };

            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            if (ImGui::Button("Toggle Log Stream", ImVec2(ImGui::GetWindowSize().x * 0.85f, 0.0f)))
            {
                config.toggleLogStream = !config.toggleLogStream;
            };

            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            if (ImGui::Button("Toggle Collisions", ImVec2(ImGui::GetWindowSize().x * 0.85f, 0.0f)))
            {
                config.shouldDisableCollisions = !config.shouldDisableCollisions;
            };

            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            if (ImGui::Button("Toggle Gravity", ImVec2(ImGui::GetWindowSize().x * 0.85f, 0.0f)))
            {
                config.shouldHandleGravity = !config.shouldHandleGravity;
            };
        }

        if (ImGui::CollapsingHeader("Configuration"))
        {
            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            ImGui::Text("Show Collider Boxes:");
            ImGui::SameLine();
            if (config.showColliderBoxes)
                ImGui::TextColored(accentColor, "YES");
            else
                ImGui::TextColored(textColor, "NO");

            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            ImGui::Text("Collisions:");
            ImGui::SameLine();
            if (!config.shouldDisableCollisions)
                ImGui::TextColored(accentColor, "YES");
            else
                ImGui::TextColored(textColor, "NO");

            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            ImGui::Text("Show Log Stream:");
            ImGui::SameLine();
            if (config.toggleLogStream)
                ImGui::TextColored(accentColor, "YES");
            else
                ImGui::TextColored(textColor, "NO");

            cursor = ImGui::GetCursorPos();
            cursor.x += 4.0f;
            ImGui::SetCursorPos(cursor);
            ImGui::Text("Handle Gravity");
            ImGui::SameLine();
            if (config.shouldHandleGravity)
                ImGui::TextColored(accentColor, "YES");
            else
                ImGui::TextColored(textColor, "NO");
        }

        ImGui::Spacing();
        ImGui::Text("Frametime:");
        ImGui::SameLine();
        ImGui::TextColored(accentColor, "%.3fms", dt);

        mainWindowXSize = ImGui::GetWindowSize().x;
        mainWindowYPos = ImGui::GetWindowPos().y;

        ImGui::End();
    }
}