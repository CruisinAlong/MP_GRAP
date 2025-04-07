#pragma once
#include "MyCamera.h"

class PerspectiveCamera : public MyCamera {
public:
    PerspectiveCamera(glm::vec3 pos, glm::vec3 rot, float fov, float aspect, float near, float far)
        : MyCamera(pos), fov(fov), aspect(aspect), near(near), far(far) {
        this->rotation = rot;
    }

    glm::mat4 getViewMatrix() {
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::translate(view, -position);
        return view;
    }

    virtual glm::mat4 getProjectionMatrix() const override {
        return glm::perspective(glm::radians(fov), aspect, near, far);
    }

    void Inputs(GLFWwindow* window);

private:
    float fov, aspect, near, far; 
};
