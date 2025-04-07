// PerspectiveCamera.cpp
#include "PerspectiveCamera.h"

void PerspectiveCamera::Inputs(GLFWwindow* window, bool use1stCam, glm::vec3 playerPos) {
    if (use1stCam) {
        // First-person camera
        position = playerPos + glm::vec3(0.0f, 1.0f, -1.0f); // Adjust the offset as needed
        front = glm::vec3(0.0f, 0.0f, -1.0f);
    }
    else {
        // Third-person camera
        double Xinput, Yinput;
        int width, height;
        float Xmod, Ymod;

        glfwGetCursorPos(window, &Xinput, &Yinput);
        glfwGetWindowSize(window, &width, &height);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            Xmod = (Xinput - (width / 2)) / width;
            Ymod = (Yinput - (height / 2)) / height;

            yaw += Xmod * 100.0f; // Adjust sensitivity as needed
            pitch += Ymod * 100.0f; // Adjust sensitivity as needed

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            front = glm::normalize(front);
        }

        position = playerPos - front * 5.0f + glm::vec3(0.0f, 2.0f, 0.0f); // Adjust the offset as needed
    }
}
