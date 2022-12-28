#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"

namespace glimac {
        
    void TrackballCamera::moveFront(float delta) {
        this->m_fDistance += delta;
    }

    void TrackballCamera::rotateLeft(float degrees) {
        this->m_fAngleX += degrees;
    }

    void TrackballCamera::rotateUp(float degrees) {
        this->m_fAngleY += degrees;
    }

    glm::mat4 TrackballCamera::getViewMatrix() const {
        glm::mat4 VMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -this->m_fDistance));
        VMatrix = glm::rotate(VMatrix, glm::radians(this->m_fAngleX), glm::vec3(0, 1, 0));
        VMatrix = glm::rotate(VMatrix, glm::radians(this->m_fAngleY), glm::vec3(1, 0, 0));
        return VMatrix;
    }

}