#pragma once

#include <vector>
#include "common.hpp"
#include "ObjectBase.hpp"

namespace glimac {
    
// Représente un cylindre ouvert discrétisé dont la base est centrée en (0, 0, 0) (dans son repère local)
class Cylinder : public ObjectBase {
    // Alloue et construit les données (implantation dans le .cpp)
    void build(GLfloat height, GLfloat radius, GLsizei discLat, GLsizei discHeight);

public:
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Cylinder(GLfloat height, GLfloat radius, GLsizei discLat, GLsizei discHeight): ObjectBase() {
        build(height, radius, discLat, discHeight); // Construction (voir le .cpp)
        this->init();
    }
};
    
}
