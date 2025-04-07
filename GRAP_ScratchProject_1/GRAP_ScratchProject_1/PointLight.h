// PointLight.h
#pragma once
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class PointLight : public Light {
public:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    PointLight(glm::vec3 ambColor, float ambStrength, glm::vec3 diffColor, glm::vec3 specColor, glm::vec3 pos, float specStr, float specPhong, float cons, float lin, float quad)
        : Light(diffColor, 1.0f), position(pos), constant(cons), linear(lin), quadratic(quad), ambient(ambColor), diffuse(diffColor), specular(specColor) {
    }

    void applyLight(GLuint shaderProgram) const override {
        // Default implementation for the base class method
        applyLight(shaderProgram, 0);
    }

    void applyLight(GLuint shaderProgram, int index) const {
        // Set the light properties in the shader
        std::string indexStr = std::to_string(index);
        glUniform3fv(glGetUniformLocation(shaderProgram, ("pointLights[" + indexStr + "].position").c_str()), 1, glm::value_ptr(position));
        glUniform1f(glGetUniformLocation(shaderProgram, ("pointLights[" + indexStr + "].constant").c_str()), constant);
        glUniform1f(glGetUniformLocation(shaderProgram, ("pointLights[" + indexStr + "].linear").c_str()), linear);
        glUniform1f(glGetUniformLocation(shaderProgram, ("pointLights[" + indexStr + "].quadratic").c_str()), quadratic);
        glUniform3fv(glGetUniformLocation(shaderProgram, ("pointLights[" + indexStr + "].ambient").c_str()), 1, glm::value_ptr(ambient));
        glUniform3fv(glGetUniformLocation(shaderProgram, ("pointLights[" + indexStr + "].diffuse").c_str()), 1, glm::value_ptr(diffuse));
        glUniform3fv(glGetUniformLocation(shaderProgram, ("pointLights[" + indexStr + "].specular").c_str()), 1, glm::value_ptr(specular));
    }
};
