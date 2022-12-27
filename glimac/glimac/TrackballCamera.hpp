#pragma once

#include "common.hpp"

namespace glimac {

    class TrackballCamera {
        
        public :
            TrackballCamera() : m_fDistance(5), m_fAngleX(0), m_fAngleY(0) {}

            void moveFront(float delta);
            void rotateLeft(float degrees);
            void rotateUp(float degrees);

            glm::mat4 getViewMatrix() const;

        private :
            float m_fDistance, m_fAngleX, m_fAngleY;
    };

}