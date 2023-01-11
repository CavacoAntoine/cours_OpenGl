#pragma once

#include <vector>

#include "common.hpp"

namespace glimac {

// Représente un pavé droit
class Pad {
    // Alloue et construit les données (implantation dans le .cpp)
    void build(GLfloat height, GLfloat length, GLfloat width);

public:
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Pad(GLfloat height, GLfloat length, GLfloat width):
        m_nVertexCount(0) {
        build(height, length, width); // Construction (voir le .cpp)
    }

    // Renvoit le pointeur vers les données
    const ShapeVertex* getDataPointer() const {
        return &m_Vertices[0];
    }
    
    // Renvoit le nombre de vertex
    GLsizei getVertexCount() const {
        return m_nVertexCount;
    }

private:
    std::vector<ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount; // Nombre de sommets
};
    
}