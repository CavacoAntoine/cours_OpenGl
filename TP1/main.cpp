#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>

const GLuint VERTEX_ATTR_POSITION = 3;
const GLuint VERTEX_ATTR_COLOR = 8;

int window_width  = 1280;
int window_height = 720;

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

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;

    Vertex2DColor() {
    }

    Vertex2DColor(glm::vec2 position, glm::vec3 color) : position(position), color(color) {
    }
};

int main(int argc, char *argv[])
{

    int pouet = argc;
    pouet++;

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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP1", nullptr, nullptr);
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
    glimac::Program program = loadProgram(applicationPath.dirPath() + "TP1/shaders/triangle.vs.glsl",
                              applicationPath.dirPath() + "TP1/shaders/triangle.fs.glsl");
    program.use();

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    Vertex2DColor vertices[] = { Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(1, 0, 0)),
    Vertex2DColor(glm::vec2(-0.5, 0.5), glm::vec3(0, 1, 0)),
    Vertex2DColor(glm::vec2(0.5, 0.5), glm::vec3(0, 0, 1)),
    Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(1, 0, 0)),
    Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0)),
    Vertex2DColor(glm::vec2(0.5, 0.5), glm::vec3(0, 0, 1))
    };
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor) , (const GLvoid*)offsetof(Vertex2DColor, position));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor) , (const GLvoid*)offsetof(Vertex2DColor, color));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    return 0;
}