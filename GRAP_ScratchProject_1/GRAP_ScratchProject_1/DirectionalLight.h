// DirectionalLight.h
#pragma once
#include "Light.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class DirectionalLight : public Light {
public:
    glm::vec3 lightPos; // This will be used as the light direction
    glm::vec3 lightColor;
    glm::vec3 ambientColor;
    float ambientStr;
    float specStr;
    float specPhong;

    DirectionalLight(glm::vec3 ambColor, glm::vec3 lightCol, glm::vec3 pos, float ambStrength, float specStrength, float shininess)
        : Light(lightCol, 1.0f), ambientColor(ambColor), lightColor(lightCol), lightPos(pos), ambientStr(ambStrength), specStr(specStrength), specPhong(shininess) {
    }

    void applyLight(GLuint shaderProgram) const override {
        GLuint lightPosAddress = glGetUniformLocation(shaderProgram, "dirLight.direction");
        glUniform3fv(lightPosAddress, 1, glm::value_ptr(lightPos));
        GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "dirLight.diffuse");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));
        GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "dirLight.ambient");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));
        GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);
        GLuint specStrAddress = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrAddress, specStr);
        GLuint specPhongAddress = glGetUniformLocation(shaderProgram, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        // Debug log to ensure the values are set correctly
        static bool debugLogged = false;
        if (!debugLogged) {
            std::cout << "DirectionalLight applied with the following values:" << std::endl;
            std::cout << "Light Position (Direction): " << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << std::endl;
            std::cout << "Light Color: " << lightColor.r << ", " << lightColor.g << ", " << lightColor.b << std::endl;
            std::cout << "Ambient Color: " << ambientColor.r << ", " << ambientColor.g << ", " << ambientColor.b << std::endl;
            std::cout << "Ambient Strength: " << ambientStr << std::endl;
            std::cout << "Specular Strength: " << specStr << std::endl;
            std::cout << "Specular Phong: " << specPhong << std::endl;
            debugLogged = true;
        }
    }
};

