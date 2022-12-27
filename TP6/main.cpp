#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Cylinder.hpp>
#include <glimac/common.hpp>
#include <glimac/glm.hpp>
#include <glimac/getTime.hpp>
#include <glimac/Image.hpp>
#include <glm/gtc/random.hpp>
#include <vector>
#include <iomanip>

int window_width  = 1280;
int window_height = 720;

const GLuint VERTEX_ATTR_POSITION = 0;
const GLuint VERTEX_ATTR_COLOR = 1;
const GLuint VERTEX_ATTR_TEXCOORDS = 2;
const GLuint VERTEX_ATTR_NORMAL = 3;

static void key_callback(GLFWwindow* /*window*/, int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
{
}

static void mouse_button_callback(GLFWwindow* /*window*/, int /*button*/, int /*action*/, int /*mods*/)
{
}

static void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double /*yoffset*/)
{
}

static void cursor_position_callback(GLFWwindow* /*window*/, double /*xpos*/, double /*ypos*/)
{
}

static void size_callback(GLFWwindow* /*window*/, int width, int height)
{
    window_width  = width;
    window_height = height;
}

struct EarthProgram {
    glimac::Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "TP6/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "TP6/shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture2");
    }
};

struct MoonProgram {
    glimac::Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    MoonProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "TP6/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "TP6/shaders/tex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
};

int main(int argc, char * argv[])
{

    if(argc < 1 ) {
        std::cout << "chemin d'accès requis\n";
        return -1;
    }

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a window and its OpenGL context */
#ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP6", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Intialize glad (loads the OpenGL functions) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    /* Hook input callbacks */
    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);

    glimac::FilePath applicationPath(argv[0]);
    EarthProgram earthProgram(applicationPath);
    MoonProgram moonProgram(applicationPath);

    std::unique_ptr<glimac::Image> earth = glimac::loadImage(applicationPath.dirPath() + "/assets/textures/EarthMap.jpg");
    if(earth == NULL){
        std::cout <<"Problème chargement image earth";
        return -1;
    }

    std::unique_ptr<glimac::Image> moon = glimac::loadImage(applicationPath.dirPath() + "/assets/textures/MoonMap.jpg");
    if(moon == NULL){
        std::cout <<"Problème chargement image moon";
        return -1;
    }

    std::unique_ptr<glimac::Image> cloud = glimac::loadImage(applicationPath.dirPath() + "/assets/textures/CloudMap.jpg");
    if(cloud == NULL){
        std::cout <<"Problème chargement image cloud";
        return -1;
    }

    GLuint tEarth, tMoon, tCloud;
    glGenTextures(1, &tEarth);
    glBindTexture(GL_TEXTURE_2D, tEarth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earth -> getWidth(), earth -> getHeight(), 0, GL_RGBA, GL_FLOAT, earth -> getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &tMoon);
    glBindTexture(GL_TEXTURE_2D, tMoon);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, moon -> getWidth(), moon -> getHeight(), 0, GL_RGBA, GL_FLOAT, moon -> getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &tCloud);
    glBindTexture(GL_TEXTURE_2D, tCloud);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloud -> getWidth(), cloud -> getHeight(), 0, GL_RGBA, GL_FLOAT, cloud -> getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(1,0);

    glimac::Sphere sphere = glimac::Sphere(1,32,32);

    // Création du vbo
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(glimac::ShapeVertex),(glimac::ShapeVertex *) sphere.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Création du vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST); // Permet l'activation de profondeur du GPU

    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (float)window_width/(float)window_height, 0.1f, 100.f);
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    std::vector<glm::vec3> random;
    std::vector<int> randomP;
    for (int i = 0; i<32; i++) {
        random.push_back(glm::sphericalRand<float>(5.f));
        for(int j = 0; j<3; j++) {
            int rand = glm::linearRand(-2,2);
            while( rand < 1 && rand >-1 ){
                rand = glm::linearRand(-2,2);
            }
            randomP.push_back(rand);
        }
    }
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.75f, 0.75f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);

        earthProgram.m_Program.use();
        
        glUniform1i(earthProgram.uEarthTexture, 0);
        glUniform1i(earthProgram.uCloudTexture, 1);

        // Earth
        MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0,0,-5));
        //MVMatrix = glm::rotate(MVMatrix, 90.f, glm::vec3(1, 0, 0)); // this line for rotate the cylinder
        MVMatrix = glm::rotate(MVMatrix, glimac::getTime(), glm::vec3(0, 1, 0)); // and change here to z axis for rotate cylinder
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tEarth);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tCloud);

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        moonProgram.m_Program.use();
        for(int i = 0; i<32; i++) {
            // Moon
            glUniform1i(moonProgram.uTexture, 0);

            glm::mat4 moonMVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5)); // Translation
            moonMVMatrix = glm::rotate(moonMVMatrix, glimac::getTime(), glm::normalize(random[i])); // Translation * Rotation
            moonMVMatrix = glm::translate(moonMVMatrix, glm::vec3(randomP[i], randomP[i+1], randomP[i+2])); // Translation * Rotation * Translation
            moonMVMatrix = glm::scale(moonMVMatrix, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
            NormalMatrix = glm::transpose(glm::inverse(moonMVMatrix));
            glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * moonMVMatrix));
            glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(moonMVMatrix));
            glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tMoon);

            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glDeleteTextures(1, &tEarth);
    glDeleteTextures(1, &tMoon);
    glDeleteTextures(1, &tCloud);
    glfwTerminate();
    return 0;
}