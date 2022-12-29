#pragma once

#include "common.hpp"

namespace glimac {

    class FreeflyCamera {
        
        public :
            FreeflyCamera() : m_Position(glm::vec3(0,0,0)), m_fPhi(M_PI), m_fTheta(0.f)
            {
                computeDirectionVectors();
            };

            void computeDirectionVectors();
            void moveLeft(float t);
            void moveFront(float t);
            void rotateLeft(float degrees);
            void rotateUp(float degrees);

            glm::mat4 getViewMatrix() const;

        private :
            glm::vec3 m_Position, m_FrontVector, m_LeftVector, m_UpVector;
            float m_fPhi, m_fTheta;
    };

}