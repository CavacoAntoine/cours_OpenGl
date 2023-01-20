#include "glimac/Texture.hpp"

Texture::Texture(const std::string& filepath, Material mat) : material(mat) {
    std::unique_ptr<glimac::Image> image = glimac::loadImage(filepath);
    if (!image) {
        std::cout << "Failed to load image: " << filepath << std::endl;
        return;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_FLOAT, image->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getID() const {
    return m_id;
}

Material Texture::getMaterial() const {
    return this->material;
}