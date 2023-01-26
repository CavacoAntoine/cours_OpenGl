#pragma once

#include "glimac/common.hpp"
#include "glimac/EditProgram.h"

struct DirLight {
    glm::vec3 direction;
  
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
        direction(direction), ambient(ambient), diffuse(diffuse), specular(specular) {};

    void setLight(EditProgram *program, glm::mat4 VMatrix) {
        glm::vec4 viewDirection = VMatrix * glm::vec4(direction, 0);
        glUniform3f(program->getLocation("uDirLight.direction"), viewDirection.x, viewDirection.y, viewDirection.z);
        glUniform3f(program->getLocation("uDirLight.ambient"), ambient.x, ambient.y, ambient.z);
        glUniform3f(program->getLocation("uDirLight.diffuse"), diffuse.x, diffuse.y, diffuse.z);
        glUniform3f(program->getLocation("uDirLight.specular"), specular.x, specular.y, specular.z);
    }
};

struct PointLight {    
    glm::vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
        position(position), constant(constant), linear(linear), quadratic(quadratic), ambient(ambient), diffuse(diffuse), specular(specular) {};

    void setLight(EditProgram *program, glm::mat4 VMatrix, int numero) {
        if(numero < 0) return;
        glm::vec4 viewPosition = VMatrix * glm::vec4(position, 1);
        glUniform3f(program->getLocation("uPointLights[" + std::to_string(numero)  + "].position"), viewPosition.x, viewPosition.y, viewPosition.z);
        
        glUniform1f(program->getLocation("uPointLights[" + std::to_string(numero)  + "].constant"), constant);
        glUniform1f(program->getLocation("uPointLights[" + std::to_string(numero)  + "].linear"), linear);
        glUniform1f(program->getLocation("uPointLights[" + std::to_string(numero)  + "].quadratic"), quadratic);

        glUniform3f(program->getLocation("uPointLights[" + std::to_string(numero)  + "].ambient"), ambient.x, ambient.y, ambient.z);
        glUniform3f(program->getLocation("uPointLights[" + std::to_string(numero)  + "].diffuse"), diffuse.x, diffuse.y, diffuse.z);
        glUniform3f(program->getLocation("uPointLights[" + std::to_string(numero)  + "].specular"), specular.x, specular.y, specular.z);
    }
};

struct SpotLight {
    glm::vec3  position;
    glm::vec3  direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;  

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
        position(position), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff), constant(constant), linear(linear), quadratic(quadratic), ambient(ambient), diffuse(diffuse), specular(specular) {};

    void setLight(EditProgram *program, glm::mat4 VMatrix, int numero) {
        if(numero < 0) return;
        glm::vec4 viewPosition = VMatrix * glm::vec4(position, 1);
        glm::vec4 viewDirection = VMatrix * glm::vec4(direction, 0);
        glUniform3f(program->getLocation("uSpotLights[" + std::to_string(numero)  + "].position"), viewPosition.x, viewPosition.y, viewPosition.z);
        glUniform3f(program->getLocation("uSpotLights[" + std::to_string(numero)  + "].direction"), viewDirection.x, viewDirection.y, viewDirection.z);

        glUniform1f(program->getLocation("uSpotLights[" + std::to_string(numero)  + "].cutOff"), cutOff);
        glUniform1f(program->getLocation("uSpotLights[" + std::to_string(numero)  + "].outerCutOff"), outerCutOff);

        glUniform1f(program->getLocation("uSpotLights[" + std::to_string(numero)  + "].constant"), constant);
        glUniform1f(program->getLocation("uSpotLights[" + std::to_string(numero)  + "].linear"), linear);
        glUniform1f(program->getLocation("uSpotLights[" + std::to_string(numero)  + "].quadratic"), quadratic);

        glUniform3f(program->getLocation("uSpotLights[" + std::to_string(numero)  + "].ambient"), ambient.x, ambient.y, ambient.z);
        glUniform3f(program->getLocation("uSpotLights[" + std::to_string(numero)  + "].diffuse"), diffuse.x, diffuse.y, diffuse.z);
        glUniform3f(program->getLocation("uSpotLights[" + std::to_string(numero)  + "].specular"), specular.x, specular.y, specular.z);
    }
}; 