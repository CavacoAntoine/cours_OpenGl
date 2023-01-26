#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Pad.hpp"

namespace glimac {

    void Pad::build(GLfloat height, GLfloat length, GLfloat width) {
        
        ShapeVertex vertex;
        
        // Construit l'ensemble des vertex
        // Face A Triangle 1
        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0;
        vertex.normal = glm::vec3(0,0,-1);
        vertex.position = glm::vec3(0, 0, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0;
        vertex.normal = glm::vec3(0,0,-1);
        vertex.position = glm::vec3(length, 0, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.25;
        vertex.normal = glm::vec3(0,0,-1);
        vertex.position = glm::vec3(0, height, 0);
        m_Vertices.push_back(vertex);

        // Face A Triangle 2
        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.25;
        vertex.normal = glm::vec3(0,0,-1);
        vertex.position = glm::vec3(0, height, 0);
        m_Vertices.push_back(vertex);
        
        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0.25;
        vertex.normal = glm::vec3(0,0,-1);
        vertex.position = glm::vec3(length, height, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0;
        vertex.normal = glm::vec3(0,0,-1);
        vertex.position = glm::vec3(length, 0, 0);
        m_Vertices.push_back(vertex);


        // Face B Triangle 1
        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(1,0,0);
        vertex.position = glm::vec3(length, 0, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(1,0,0);
        vertex.position = glm::vec3(length, height, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 1;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(1,0,0);
        vertex.position = glm::vec3(length, 0, width);
        m_Vertices.push_back(vertex);

        // Face B Triangle 2
        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(1,0,0);
        vertex.position = glm::vec3(length, height, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 1;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(1,0,0);
        vertex.position = glm::vec3(length, 0, width);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 1;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(1,0,0);
        vertex.position = glm::vec3(length, height, width);
        m_Vertices.push_back(vertex);
        

        // Face C Triangle 1
        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(0,0,1);
        vertex.position = glm::vec3(length, 0, width);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(0,0,1);
        vertex.position = glm::vec3(0, 0, width);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(0,0,1);
        vertex.position = glm::vec3(length, height, width);
        m_Vertices.push_back(vertex);

        // Face C Triangle 2
        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(0,0,1);
        vertex.position = glm::vec3(length, height, width);
        m_Vertices.push_back(vertex);
        
        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(0,0,1);
        vertex.position = glm::vec3(0, height, width);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(0,0,1);
        vertex.position = glm::vec3(0, 0, width);
        m_Vertices.push_back(vertex);


        // Face D Triangle 1
        vertex.texCoords.x = 0;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(-1,0,0);
        vertex.position = glm::vec3(0, 0, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(-1,0,0);
        vertex.position = glm::vec3(0, height, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(-1,0,0);
        vertex.position = glm::vec3(0, 0, width);
        m_Vertices.push_back(vertex);

        // Face D Triangle 2
        vertex.texCoords.x = 0;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(-1,0,0);
        vertex.position = glm::vec3(0, height, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(-1,0,0);
        vertex.position = glm::vec3(0, 0, width);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(-1,0,0);
        vertex.position = glm::vec3(0, height, width);
        m_Vertices.push_back(vertex);


        // Face E Triangle 1
        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.25;
        vertex.normal = glm::vec3(0,1,0);
        vertex.position = glm::vec3(0, height, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0.25;
        vertex.normal = glm::vec3(0,1,0);
        vertex.position = glm::vec3(length, height, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(0,1,0);
        vertex.position = glm::vec3(0, height, width);
        m_Vertices.push_back(vertex);

        // Face E Triangle 2
        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(0,1,0);
        vertex.position = glm::vec3(0, height, width);
        m_Vertices.push_back(vertex);
        
        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0.5;
        vertex.normal = glm::vec3(0,1,0);
        vertex.position = glm::vec3(length, height, width);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0.25;
        vertex.normal = glm::vec3(0,1,0);
        vertex.position = glm::vec3(length, height, 0);
        m_Vertices.push_back(vertex);


        // Face F Triangle 1
        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 1;
        vertex.normal = glm::vec3(0,-1,0);
        vertex.position = glm::vec3(0, 0, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(0,-1,0);
        vertex.position = glm::vec3(0, 0, width);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 1;
        vertex.normal = glm::vec3(0,-1,0);
        vertex.position = glm::vec3(length, 0, 0);
        m_Vertices.push_back(vertex);

        // Face F Triangle 2
        vertex.texCoords.x = 0.3333;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(0,-1,0);
        vertex.position = glm::vec3(0, 0, width);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 1;
        vertex.normal = glm::vec3(0,-1,0);
        vertex.position = glm::vec3(length, 0, 0);
        m_Vertices.push_back(vertex);

        vertex.texCoords.x = 0.6666;
        vertex.texCoords.y = 0.75;
        vertex.normal = glm::vec3(0,-1,0);
        vertex.position = glm::vec3(length, 0, width);
        m_Vertices.push_back(vertex);

        m_nVertexCount = 36;
    }

    

}
