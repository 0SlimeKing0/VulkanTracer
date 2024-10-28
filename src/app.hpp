#pragma once

//#define NDEBUG
#include "lve_pipeline.hpp"
#include "lve_swapchain.hpp"
#include "cam.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>

namespace lve {
class App {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    PushConstant push = {glm::mat4(1.0f), (float)WIDTH / (float)HEIGHT, glm::vec3(0.0f)};

    App();

    App(const App &) = delete;
    App &operator=(const App &) = delete;

    void Run();
private:
    void CreatePipelineLayout();
    void CreatePipeline();
    void CreateCommandBuffers();
    void DrawFrame();
    void RecreateSwapChain();
    void RecordCommandBuffer(int imageIndex);

    LveWindow lveWindow{WIDTH, HEIGHT, "g"};
    LveDevice lveDevice{lveWindow};
    Cam cam{};

    std::unique_ptr<LveSwapChain> lveSwapChain;
    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;

    void CleanUp();
};
}