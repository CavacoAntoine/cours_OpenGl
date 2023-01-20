#include "glimac/EditProgram.h"

void EditProgram::adduLocation(std::string name) {
    this->uLocation[name] = glGetUniformLocation(this->m_Program.getGLId(), &name[0]);
}

GLuint EditProgram::getLocation(std::string name) {
    return this->uLocation[name];
}

void EditProgram::setMaterial(Material material) {
    glUniform3f(this->getLocation("material.ambient"), material.ambient.x, material.ambient.y, material.ambient.z);
    glUniform3f(this->getLocation("material.diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z);
    glUniform3f(this->getLocation("material.specular"), material.specular.x, material.specular.y, material.specular.z);
    glUniform1f(this->getLocation("material.shininess"), material.shininess);    
}

void LightsTextsProgram::setMaterial(Material material) {
    EditProgram::setMaterial(material);
    for(int i = 0; i < this->nbrTextures; i++) {
        glUniform1i(this->getLocation(std::string("uTextures[" + std::to_string(i) + "]")), i);
    }
}