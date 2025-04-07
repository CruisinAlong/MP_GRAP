#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Light {
public:
    glm::vec3 color;
    float intensity;

    Light(glm::vec3 col, float inten) : color(col), intensity(inten) {}

    virtual void applyLight(GLuint shaderProgram) const = 0;
};
