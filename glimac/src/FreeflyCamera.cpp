#include "glimac/FreeflyCamera.hpp"
#include "glimac/common.hpp"
#include <iostream>

namespace glimac {


    void FreeflyCamera::computeDirectionVectors() {
        this->m_FrontVector =  glm::vec3(glm::cos(this->m_fTheta) * glm::sin(this->m_fPhi), glm::sin(this->m_fTheta), glm::cos(this->m_fTheta) * glm::cos(this->m_fPhi));
        this->m_LeftVector = glm::vec3(glm::sin(this->m_fPhi + M_PI_2), 0, glm::cos(this->m_fPhi + M_PI_2));
        this->m_UpVector = glm::cross(this->m_FrontVector, this->m_LeftVector);
        
    }

    void FreeflyCamera::moveLeft(float t) {
        this->m_Position += (t * m_LeftVector);
    }

    void FreeflyCamera::moveFront(float t) {
        this->m_Position += (t * m_FrontVector);
        
    }

    void FreeflyCamera::moveUp(float t) {
        this->m_Position += (t * m_UpVector);
    }

    void FreeflyCamera::rotateLeft(float degrees) {
        this->m_fPhi += glm::radians(degrees);
        this->computeDirectionVectors();
    }

    void FreeflyCamera::rotateUp(float degrees) {
        this->m_fTheta += glm::radians(degrees);
        this->computeDirectionVectors();
    }

    glm::mat4 FreeflyCamera::getViewMatrix() const {
        return glm::lookAt(this->m_Position, this->m_Position + this->m_FrontVector, this->m_UpVector);
    }

}