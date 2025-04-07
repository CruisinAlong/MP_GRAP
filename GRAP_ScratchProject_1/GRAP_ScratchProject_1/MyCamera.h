#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class MyCamera {
public:
    glm::vec3 position;
    glm::vec3 rotation;

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    glm::vec3 globalUp;

    float yaw, pitch, speed;

    MyCamera(glm::vec3 pos, glm::vec3 rot) : position(pos), rotation(rot) {
    
        updateCamera();
    
    }

    MyCamera(glm::vec3 pos) : position(pos),
        globalUp(glm::vec3(0.0f, 1.0f, 0.0f)),
        yaw(-90.0f),
        pitch(0.0f),
        speed(3.0f),
        front(glm::vec3(0.0f, 0.0f, -1.0f))
    {

        updateCamera();

    }

    /*virtual glm::mat4 getViewMatrix() const {
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::translate(view, -position);
        return view;
    }*/

    virtual glm::mat4 getProjectionMatrix() const = 0;

    glm::mat4 ViewMatrix() {

        return glm::lookAt(position, position + front, up);

    }

    //void updateDir(double x, double y) {

    //    yaw += x;
    //    pitch += y;

    //    //Pitch correction.
    //    if (pitch > 89.0f) {

    //        pitch = 89.0f;

    //    }
    //    else if (pitch < -89.0f) {

    //        pitch = -89.0f;

    //    }

    //}

    //Converts pitch and yaw to directional information
    void updateCamera() {

        //Direction of camera.
        glm::vec3 dir;

        //SohCahToa stuff to calculate direction based on pitch and yaw.
        dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        dir.y = sin(glm::radians(pitch));
        dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        //Getting directions.
        front = glm::normalize(dir);
        right = glm::normalize(glm::cross(front, globalUp));
        up = glm::normalize(glm::cross(right, front));

    }

};
