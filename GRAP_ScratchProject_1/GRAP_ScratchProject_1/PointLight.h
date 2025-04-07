#pragma once
#include "Light.h"
#include <iostream>

class PointLight : public Light {
public:
    glm::vec3 position;
    float ambientStrength;
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float specularStrength;
    float specularPhong;
    float constant, linear, quadratic;

    PointLight(glm::vec3 ambColor, float ambStrength, glm::vec3 diffColor, glm::vec3 specColor, glm::vec3 pos, float specStr, float specPhong, float cons, float lin, float quad)
        : Light(diffColor, 1.0f), ambientStrength(ambStrength), ambientColor(ambColor), diffuseColor(diffColor), specularColor(specColor), position(pos), specularStrength(specStr), specularPhong(specPhong), constant(cons), linear(lin), quadratic(quad) {
    }

    void applyLight(GLuint shaderProgram) const override {
        static bool debugLogged = false;

        GLuint lightPositionAddress = glGetUniformLocation(shaderProgram, "pointLightPos");
        glUniform3fv(lightPositionAddress, 1, glm::value_ptr(position));
        GLuint ambientStrengthAddress = glGetUniformLocation(shaderProgram, "pointLightAmbientStr");
        glUniform1f(ambientStrengthAddress, ambientStrength);
        GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "pointLightColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));
        GLuint diffuseColorAddress = glGetUniformLocation(shaderProgram, "pointLightDiffuseColor");
        glUniform3fv(diffuseColorAddress, 1, glm::value_ptr(diffuseColor));
        GLuint specularColorAddress = glGetUniformLocation(shaderProgram, "pointLightSpecularColor");
        glUniform3fv(specularColorAddress, 1, glm::value_ptr(specularColor));
        GLuint specularStrengthAddress = glGetUniformLocation(shaderProgram, "pointLightSpecStr");
        glUniform1f(specularStrengthAddress, specularStrength);
        GLuint specularPhongAddress = glGetUniformLocation(shaderProgram, "pointLightSpecPhong");
        glUniform1f(specularPhongAddress, specularPhong);

        GLuint constantAddress = glGetUniformLocation(shaderProgram, "pointLightConstant");
        glUniform1f(constantAddress, constant);
        GLuint linearAddress = glGetUniformLocation(shaderProgram, "pointLightLinear");
        glUniform1f(linearAddress, linear);
        GLuint quadraticAddress = glGetUniformLocation(shaderProgram, "pointLightQuadratic");
        glUniform1f(quadraticAddress, quadratic);

        if (!debugLogged) {
            std::cout << "PointLight applied with the following values:" << std::endl;
            std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
            std::cout << "Ambient Strength: " << ambientStrength << std::endl;
            std::cout << "Ambient Color: " << ambientColor.r << ", " << ambientColor.g << ", " << ambientColor.b << std::endl;
            std::cout << "Diffuse Color: " << diffuseColor.r << ", " << diffuseColor.g << ", " << diffuseColor.b << std::endl;
            std::cout << "Specular Color: " << specularColor.r << ", " << specularColor.g << ", " << specularColor.b << std::endl;
            std::cout << "Specular Strength: " << specularStrength << std::endl;
            std::cout << "Specular Phong: " << specularPhong << std::endl;
            std::cout << "Constant: " << constant << ", Linear: " << linear << ", Quadratic: " << quadratic << std::endl;
            debugLogged = true;
        }
    }
};

