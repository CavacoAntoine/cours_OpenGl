#pragma once

#include <vector>

#include "common.hpp"
#include "ObjectBase.hpp"

namespace glimac {

// Représente un pavé droit
class Pad : public ObjectBase {
    // Alloue et construit les données (implantation dans le .cpp)
    void build(GLfloat height, GLfloat length, GLfloat width);

public:
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Pad(GLfloat height, GLfloat length, GLfloat width) : ObjectBase() {
        build(height, length, width); // Construction (voir le .cpp)
        this->init();
    }
};
    
}