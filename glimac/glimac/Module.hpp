#include <glad/glad.h>
#include "common.hpp"

class Module {

    public:
        Module();
        GLuint getVAO() const;

    private:
        std::vector<ShapeVertex> m_Vertices;
        GLsizei m_nVertexCount; // Nombre de sommets
        GLuint m_vbo, m_vao;
};