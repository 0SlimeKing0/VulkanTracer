#include "cam.hpp"

glm::vec4 AXIS_X = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
glm::vec4 AXIS_Y = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
glm::vec4 AXIS_Z = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
glm::mat4 IDENTITY = glm::mat4(1.0f);

glm::vec3 AXIS_X_3 = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 AXIS_Y_3 = glm::vec3(0.0f, 1.0f, 0.0f);

namespace lve {
void Cam::ProcessInput(GLFWwindow* window, PushConstant& push) {
    float currentFrame = glfwGetTime();
    float delta = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        push.camPos += glm::vec3(AXIS_X * push.rotMat * camSpeed * delta);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        push.camPos += glm::vec3(-AXIS_Z * push.rotMat * camSpeed * delta);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        push.camPos += glm::vec3(-AXIS_X * push.rotMat * camSpeed * delta);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        push.camPos += glm::vec3(AXIS_Z * push.rotMat * camSpeed * delta);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camPitch += delta * cameraRotSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camPitch -= delta * cameraRotSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camYaw += delta * cameraRotSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camYaw -= delta * cameraRotSpeed;

    if (camYaw < 0.0f) camYaw += 360.0f;
    if (camYaw > 360.0f) camYaw -= 360.0f;

    if (camPitch > 89.0f) camPitch = 89.0f;
    if (camPitch < -89.0f) camPitch = -89.0f;

    float yawRadians = glm::radians(camYaw);
    float pitchRadians = glm::radians(camPitch);

    push.rotMat = glm::rotate(glm::rotate(IDENTITY, pitchRadians, AXIS_X_3), yawRadians, AXIS_Y_3);
}
}