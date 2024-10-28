#include "app.hpp"

#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <spdlog/spdlog.h>

namespace lve {
App::App() {
    CreatePipelineLayout();
    RecreateSwapChain();
    CreateCommandBuffers();
}

void App::Run() {
    while (!lveWindow.ShouldClose())
    {
        glfwPollEvents();
        cam.ProcessInput(lveWindow.window, push);
        DrawFrame();
    }

    vkDeviceWaitIdle(lveDevice.Device());

    CleanUp();
}

void App::CreatePipelineLayout() {
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PushConstant);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(lveDevice.Device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout!");
    }
}

void App::CreatePipeline() {
    auto pipelineConfig = LvePipeline::DefaultPipelineConfigInfo(lveSwapChain->Width(), lveSwapChain->Height());
    pipelineConfig.renderPass = lveSwapChain->GetRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;

    if (lvePipeline != nullptr) {
        lvePipeline->CleanUp();
    }

    lvePipeline = std::make_unique<LvePipeline>(lveDevice, "../../shaders/simple.vert.spv", "../../shaders/simple.frag.spv", pipelineConfig);
}

void App::RecreateSwapChain() {
    auto extent = lveWindow.GetExtent();

    while (extent.width == 0 || extent.height == 0)
    {
        extent = lveWindow.GetExtent();
        glfwWaitEvents();
    }
    
    vkDeviceWaitIdle(lveDevice.Device());

    if (lveSwapChain != nullptr) {
        lveSwapChain->CleanUp();
    }

    push.aspectRatio = (float)extent.width / (float)extent.height;

    lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent);
    CreatePipeline();
}

void App::CleanUp() {
    vkDestroyPipelineLayout(lveDevice.Device(), pipelineLayout, nullptr);
    
    lveSwapChain->CleanUp();
    lvePipeline->CleanUp();
    lveWindow.CleanUp();
    lveDevice.CleanUp();

    SPDLOG_INFO("Destroyed everything");
}

void App::CreateCommandBuffers() {
    commandBuffers.resize(lveSwapChain->ImageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = lveDevice.GetCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(lveDevice.Device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}

void App::RecordCommandBuffer(int imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = lveSwapChain->GetRenderPass();
    renderPassInfo.framebuffer = lveSwapChain->GetFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = lveSwapChain->GetSwapChainExtent();

    VkClearValue color = {0.0f, 0.0f, 0.0f, 1.0f};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &color;

    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    lvePipeline->Bind(commandBuffers[imageIndex]);

    vkCmdPushConstants(commandBuffers[imageIndex], pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstant), &push);
    vkCmdDraw(commandBuffers[imageIndex], 4, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}

void App::DrawFrame() {
    uint32_t imageIndex;
    auto result = lveSwapChain->AcquireNextImage(&imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapChain();
        return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    RecordCommandBuffer(imageIndex);
    result = lveSwapChain->SubmitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.WasResized()) {
        lveWindow.ResetResizedFlag();
        RecreateSwapChain();
        return;
    }

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image!");
    }
}
}
