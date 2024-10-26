#pragma once

//#define NDEBUG
#include "lve_pipeline.hpp"
#include "lve_swapchain.hpp"

#include <memory>

namespace lve {
class App {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    App();

    App(const App &) = delete;
    App &operator=(const App &) = delete;

    void Run();
private:
    void CreatePipelineLayout();
    void CreatePipeline();
    void CreateCommandBuffers();
    void DrawFrame();

    LveWindow lveWindow{WIDTH, HEIGHT, "g"};
    LveDevice lveDevice{lveWindow};
    LveSwapChain lveSwapChain{lveDevice, {WIDTH, HEIGHT}};
    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;

    void CleanUp();
};
}