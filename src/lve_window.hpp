#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULCAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <string>
#include <iostream>

namespace lve {
struct PushConstant {
    alignas(16) glm::mat4 rotMat;
    alignas(16) GLfloat aspectRatio;
    alignas(16) glm::vec3 camPos;
};

class LveWindow {
public:
    LveWindow(int w, int h, std::string name);

    LveWindow(const LveWindow &) = delete;
    LveWindow &operator=(const LveWindow &) = delete;

    GLFWwindow* window;

    void CleanUp();

    bool ShouldClose() { return glfwWindowShouldClose(window); };
    void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    VkExtent2D GetExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

    bool WasResized() { return frameBufferResized; }
    void ResetResizedFlag() { frameBufferResized = false; }
private:
    static void FrameBufferResizedCallback(GLFWwindow *window, int width, int height);

    void InitWindow();

    int width;
    int height;
    bool frameBufferResized = false;

    std::string windowName;
};
}