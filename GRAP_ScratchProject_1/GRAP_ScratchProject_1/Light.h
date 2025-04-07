#pragma once
#include <glm/glm.hpp>

class Light {
public:
    glm::vec3 color;
    float intensity;

    Light(glm::vec3 col, float inten) : color(col), intensity(inten) {}

    virtual void applyLight(GLuint shaderProgram) const = 0;
};
