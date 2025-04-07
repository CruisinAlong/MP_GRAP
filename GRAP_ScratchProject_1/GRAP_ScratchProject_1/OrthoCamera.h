#pragma once
#include "MyCamera.h"

class OrthoCamera : public MyCamera {
public:

    OrthoCamera(glm::vec3 pos, glm::vec3 rot)
        : MyCamera(pos) {
    
        this->rotation = rot;

    }

    void positionAbove();

    virtual glm::mat4 getProjectionMatrix() const override {

        return glm::lookAt(position, position + front, up);
    }

};
