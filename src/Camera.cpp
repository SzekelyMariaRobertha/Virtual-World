#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;

        //TODO - Update the rest of camera parameters DONE
        
        // Calculate the camera's front direction
        glm::vec3 front = cameraTarget - cameraPosition;
        cameraFrontDirection = glm::normalize(front);

        // Calculate the camera's right direction
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraTarget, cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //TODO DONE

        glm::vec3 inainte = glm::normalize(cameraTarget - cameraPosition);
        glm::vec3 dreapta = glm::normalize(glm::cross(inainte, cameraUpDirection));

        switch (direction) {
            case MOVE_FORWARD:
                cameraPosition += inainte * speed;
                cameraTarget = cameraPosition + inainte;
                break;
            
            case MOVE_BACKWARD:
                cameraPosition -= inainte * speed;
                cameraTarget = cameraPosition + inainte;
                break;

            case MOVE_RIGHT:
                cameraPosition += dreapta * speed;
                cameraTarget = cameraPosition + inainte;
                break;

            case MOVE_LEFT:
                cameraPosition -= dreapta * speed;
                cameraTarget = cameraPosition + inainte;
                break;

            case MOVE_UP:
                /*cameraPosition += speed;*/
                cameraPosition += glm::vec3(0.0f, 0.01f, 0.0f);
                cameraTarget += glm::vec3(0.0f, 0.01f, 0.0f);
                break;
            
            case MOVE_DOWN:
                /*cameraPosition -= speed;*/
                cameraPosition -= glm::vec3(0.0f, 0.01f, 0.0f);
                cameraTarget -= glm::vec3(0.0f, 0.01f, 0.0f);
                break;

        }
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO DONE

        glm::vec3 inainte = glm::normalize(cameraTarget - cameraPosition);
        inainte = glm::vec3(glm::rotate<float>(yaw, cameraUpDirection) * glm::vec4(inainte, 0.f));

        glm::vec3 dreapta = glm::normalize(glm::cross(inainte, cameraUpDirection));

        inainte = glm::vec3(glm::rotate<float>(pitch, dreapta) * glm::vec4(inainte, 0.f));
        inainte = glm::normalize(inainte);

        cameraTarget = cameraPosition + inainte;
    }

  
    // Getter
    glm::vec3 Camera::getPosition() {
        return cameraPosition;
    }

    glm::vec3 Camera::getTarget() {
        return cameraTarget;
    }

    // Getter
    void Camera::setPosition(glm::vec3 newPos) {
        cameraPosition = newPos;
    }

    void Camera::setTarget(glm::vec3 newTar) {
        cameraPosition = newTar;
    }
}