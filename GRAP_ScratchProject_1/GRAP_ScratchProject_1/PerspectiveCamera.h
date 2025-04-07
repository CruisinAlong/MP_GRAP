// PerspectiveCamera.h
#pragma once
#include "MyCamera.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PerspectiveCamera : public MyCamera {
public:
    float fov, aspect, near, far;

    PerspectiveCamera(glm::vec3 pos, glm::vec3 rot, float fov, float aspect, float near, float far)
        : MyCamera(pos, rot), fov(fov), aspect(aspect), near(near), far(far) {
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 getProjectionMatrix() const override {
        return glm::perspective(glm::radians(fov), aspect, near, far);
    }

    void Inputs(GLFWwindow* window, bool use1stCam, glm::vec3 playerPos);
};
