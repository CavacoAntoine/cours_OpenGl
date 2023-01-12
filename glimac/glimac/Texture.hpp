#include "common.hpp"

class Texture {
    public:
        Texture(const std::string& filepath);
        GLuint getID() const;

    private:
        GLuint m_id;
};