#include "lve_window.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace lve {
class Cam {
    public:
        Cam() {}

        void ProcessInput(GLFWwindow* window, PushConstant& push);
    private:
        const float camSpeed = 8.0f;
        const float cameraRotSpeed = 64.0f;

        float camYaw = 0.0f;
        float camPitch = 0.0f;

        float lastFrame = glfwGetTime();
};
}