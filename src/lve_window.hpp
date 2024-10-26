#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULCAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <string>
#include <iostream>

namespace lve {
class LveWindow
{
public:
    LveWindow(int w, int h, std::string name);

    LveWindow(const LveWindow &) = delete;
    LveWindow &operator=(const LveWindow &) = delete;

    GLFWwindow* window;

    void CleanUp();

    bool ShouldClose() { return glfwWindowShouldClose(window); };
    void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
private:
    void InitWindow();

    const int width;
    const int height;

    std::string windowName;
};
}