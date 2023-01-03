#include "glimac/EditProgram.h"

void EditProgram::adduLocation(std::string name) {
    this->uLocation[name] = glGetUniformLocation(this->m_Program.getGLId(), &name[0]);
}

GLuint EditProgram::getLocation(std::string name) {
    return this->uLocation[name];
}