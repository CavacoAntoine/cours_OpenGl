#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glimac/common.hpp"
#include "glimac/Image.hpp"
#include <iomanip>

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) : 
        ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
}; 

class Texture {
    public:
        Texture(const std::string& filepath, Material mat);
        GLuint getID() const;
        Material getMaterial() const;

    private:
        GLuint m_id;
        Material material;
};