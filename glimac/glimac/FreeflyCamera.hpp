#pragma once

#include "common.hpp"

namespace glimac {

    class FreeflyCamera {
        
        public :
            glm::vec3 m_Position;

            FreeflyCamera() : m_Position(glm::vec3(0,0.1,0)), m_fPhi(M_PI), m_fTheta(0.f)
            {
                computeDirectionVectors();
            };

            FreeflyCamera(glm::vec3 position) : m_Position(position), m_fPhi(M_PI), m_fTheta(0.f)
            {
                computeDirectionVectors();
            };

            void computeDirectionVectors();
            void moveLeft(float t);
            void moveFront(float t);
            void moveUp(float t);
            void rotateLeft(float degrees);
            void rotateUp(float degrees);

            glm::mat4 getViewMatrix() const;

        private :
            glm::vec3 m_FrontVector, m_LeftVector, m_UpVector;
            float m_fPhi, m_fTheta;

            void notUnderZero();
    };

}