#include "app.hpp"
#include <spdlog/spdlog.h>

namespace lve
{
void App::Run() {
    while (!lveWindow.ShouldClose())
    {
        glfwPollEvents();
    }

    CleanUp();
}

void App::CleanUp() {
    lveDevice.CleanUp();
    lveWindow.CleanUp();

    SPDLOG_INFO("Destroyed everything");
}
}
