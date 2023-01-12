#pragma once

#include <vector>
#include "common.hpp"

const GLuint VERTEX_ATTR_POSITION = 0;
const GLuint VERTEX_ATTR_COLOR = 1;
const GLuint VERTEX_ATTR_TEXCOORDS = 2;
const GLuint VERTEX_ATTR_NORMAL = 3;

class ObjectBase {

    public:
        ObjectBase() : m_nVertexCount{0} {};
        GLuint getVAO() const;

        void init();

        // Renvoit le pointeur vers les données
        const glimac::ShapeVertex* getDataPointer() const {
            return &m_Vertices[0];
        }
        
        // Renvoit le nombre de vertex
        GLsizei getVertexCount() const {
            return m_nVertexCount;
        }

    protected:
        std::vector<glimac::ShapeVertex> m_Vertices;
        GLsizei m_nVertexCount; // Nombre de sommets
    
    private:
        GLuint m_vbo, m_vao;
};