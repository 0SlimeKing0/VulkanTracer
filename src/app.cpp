#include "app.hpp"
#include <spdlog/spdlog.h>

namespace lve
{
App::App() {
    CreatePipelineLayout();
    CreatePipeline();
    CreateCommandBuffers();
}

void App::Run() {
    while (!lveWindow.ShouldClose())
    {
        glfwPollEvents();
    }

    CleanUp();
}

void App::CreatePipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(lveDevice.Device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout!");
    }
}

void App::CreatePipeline() {
    auto pipelineConfig = LvePipeline::DefaultPipelineConfigInfo(lveSwapChain.Width(), lveSwapChain.Height());
    pipelineConfig.renderPass = lveSwapChain.GetRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    lvePipeline = std::make_unique<LvePipeline>(lveDevice, "../../shaders/simple.vert.spv", "../../shaders/simple.frag.spv", pipelineConfig);
}

void App::CleanUp() {
    vkDestroyPipelineLayout(lveDevice.Device(), pipelineLayout, nullptr);

    lvePipeline->CleanUp();
    lveDevice.CleanUp();
    lveWindow.CleanUp();

    SPDLOG_INFO("Destroyed everything");
}

void App::CreateCommandBuffers() {}
void App::DrawFrame() {}
}
