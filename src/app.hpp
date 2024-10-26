#pragma once

//#define NDEBUG
#include "lve_pipeline.hpp"

namespace lve {
class App {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void Run();
private:
    LveWindow lveWindow{WIDTH, HEIGHT, "g"};
    LveDevice lveDevice{lveWindow};
    LvePipeline lvePipeline{lveDevice, "../../shaders/simple.vert.spv", "../../shaders/simple.frag.spv", LvePipeline::DefaultPipelineCreateInfo(WIDTH, HEIGHT)};

    void CleanUp();
};
}