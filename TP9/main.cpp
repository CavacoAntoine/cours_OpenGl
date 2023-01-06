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
#include <glimac/TrackballCamera.hpp>
#include <glimac/FreeflyCamera.hpp>
#include <glimac/EditProgram.h>
#include <glm/gtc/random.hpp>
#include <vector>
#include <iomanip>
#include <functional>
#include <thread>

int window_width  = 1280;
int window_height = 720;

glimac::FreeflyCamera freeflyCamera = glimac::FreeflyCamera();

bool stop_key_loop = false;
const float SPEED_TRANSLATE = 0.05;
const int NUM_KEYS = 6;
const int KEYS[NUM_KEYS] = {GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT};
bool keyPress[NUM_KEYS] = {false};

std::function<void()> keyFuncs[NUM_KEYS] = {
    std::bind(&glimac::FreeflyCamera::moveFront, &freeflyCamera, SPEED_TRANSLATE),
    std::bind(&glimac::FreeflyCamera::moveLeft, &freeflyCamera, SPEED_TRANSLATE),
    std::bind(&glimac::FreeflyCamera::moveFront, &freeflyCamera,-SPEED_TRANSLATE),
    std::bind(&glimac::FreeflyCamera::moveLeft, &freeflyCamera, -SPEED_TRANSLATE),
    std::bind(&glimac::FreeflyCamera::moveUp, &freeflyCamera, SPEED_TRANSLATE),
    std::bind(&glimac::FreeflyCamera::moveUp, &freeflyCamera, -SPEED_TRANSLATE),
};

bool isButtonRightPress = false; 
double pPos[2] = {false}; // x and y

const GLuint VERTEX_ATTR_POSITION = 0;
const GLuint VERTEX_ATTR_COLOR = 1;
const GLuint VERTEX_ATTR_TEXCOORDS = 2;
const GLuint VERTEX_ATTR_NORMAL = 3;

static void key_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
    for(int i = 0; i < NUM_KEYS; i++) {
        if(key == KEYS[i]) {
            if(action == GLFW_PRESS) {
                keyPress[i] = true;
            } else if (action == GLFW_RELEASE) {
                keyPress[i] = false;
            }
        }
    }    
}

static void key_loop() {
    while(!stop_key_loop) {
        for (int i = 0; i < NUM_KEYS; i++) {
            if(keyPress[i]) {
                keyFuncs[i]();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int /*mods*/)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !isButtonRightPress) {
        isButtonRightPress = true;
        glfwGetCursorPos(window, &pPos[0], &pPos[1]);
    } else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        isButtonRightPress = false;
    }
}

static void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double /*yoffset*/)
{
}

static void cursor_position_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
    if(isButtonRightPress) {
        freeflyCamera.rotateLeft((xpos - pPos[0])/10);
        freeflyCamera.rotateUp((ypos - pPos[1])/10);
        pPos[0] = xpos;
        pPos[1] = ypos;
    }
}

static void size_callback(GLFWwindow* /*window*/, int width, int height)
{
    window_width  = width;
    window_height = height;
}

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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP9", nullptr, nullptr);
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
    EditProgram earthProgram = LightsTextsProgram(applicationPath, "TP6/shaders/3D.vs.glsl", "TP9/shaders/Lights2Texts.fs.glsl", false, 2, 2);
    EditProgram moonProgram = LightsTextsProgram(applicationPath, "TP6/shaders/3D.vs.glsl", "TP9/shaders/LightsText.fs.glsl", false, 1, 2);

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
    glm::mat4 VMatrix;
    glm::mat4 MMatrix;
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

    /* Launch thread for input keyboard */
    std::thread thread_key(key_loop);
        
    /* Point lights */
    glm::vec4 lightPos1(0.8, 0.8, 0.8, 1);
    glm::vec4 lightPos2(-0.8, -0.8, -0.8, 1);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.75f, 0.75f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        VMatrix = freeflyCamera.getViewMatrix();

        glm::vec4 uLightPos1 = VMatrix * lightPos1;
        glm::vec4 uLightPos2 = VMatrix * lightPos2;

        glBindVertexArray(vao);

        earthProgram.m_Program.use();

        glUniform1i(earthProgram.getLocation("uNbrPointLights"), 2);

        glUniform3f(earthProgram.getLocation("uPointLights[0].position"), uLightPos1.x, uLightPos1.y, uLightPos1.z);
        glUniform3f(earthProgram.getLocation("uPointLights[0].ambient"), 0.1, 0, 0);
        glUniform3f(earthProgram.getLocation("uPointLights[0].diffuse"), 0, 0, 0);
        glUniform3f(earthProgram.getLocation("uPointLights[0].specular"), 0, 0, 0);
        glUniform1f(earthProgram.getLocation("uPointLights[0].constant"), 1.0f);
        glUniform1f(earthProgram.getLocation("uPointLights[0].linear"), 0.09f);
        glUniform1f(earthProgram.getLocation("uPointLights[0].quadratic"), 0.032f);

        glUniform3f(earthProgram.getLocation("uPointLights[1].position"), uLightPos2.x, uLightPos2.y, uLightPos2.z);
        glUniform3f(earthProgram.getLocation("uPointLights[1].ambient"), 0, 0, 0.1);
        glUniform3f(earthProgram.getLocation("uPointLights[1].diffuse"), 0, 0, 0);
        glUniform3f(earthProgram.getLocation("uPointLights[1].specular"), 0, 0, 0);
        glUniform1f(earthProgram.getLocation("uPointLights[0].constant"), 1.0f);
        glUniform1f(earthProgram.getLocation("uPointLights[0].linear"), 0.09f);
        glUniform1f(earthProgram.getLocation("uPointLights[0].quadratic"), 0.032f);

        glUniform3f(earthProgram.getLocation("uViewPos"), freeflyCamera.m_Position.x, freeflyCamera.m_Position.y, freeflyCamera.m_Position.z);
        
        glUniform3f(earthProgram.getLocation("material.ambient"), 0.4, 0.4, 0.4);
        glUniform3f(earthProgram.getLocation("material.diffuse"), 0.75, 0.75, 1.0);
        glUniform3f(earthProgram.getLocation("material.specular"), 1.0, 1.0, 1.0);
        glUniform1f(earthProgram.getLocation("material.shininess"), 10.0);

        glUniform1i(earthProgram.getLocation("uTextures[0]"), 0);
        glUniform1i(earthProgram.getLocation("uTextures[1]"), 1);

        // Earth
        MMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
        MMatrix = glm::rotate(MMatrix, glimac::getTime(), glm::vec3(0, 1, 0));
        NormalMatrix = glm::transpose(glm::inverse(VMatrix * MMatrix));
        glUniformMatrix4fv(earthProgram.getLocation("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(ProjMatrix * VMatrix * MMatrix));
        glUniformMatrix4fv(earthProgram.getLocation("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(VMatrix * MMatrix));
        glUniformMatrix4fv(earthProgram.getLocation("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tEarth);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tCloud);

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        moonProgram.m_Program.use();
        for(int i = 0; i<32; i++) {
            // Moon
            glUniform3f(moonProgram.getLocation("uPointLights[0].position"), uLightPos1.x, uLightPos1.y, uLightPos1.z);
            glUniform3f(moonProgram.getLocation("uPointLights[0].ambient"), 0.1, 0, 0);
            glUniform3f(moonProgram.getLocation("uPointLights[0].diffuse"), 0.6, 0, 0);
            glUniform3f(moonProgram.getLocation("uPointLights[0].specular"), 0.3, 0, 0.3);
            glUniform1f(moonProgram.getLocation("uPointLights[0].constant"), 1.0f);
            glUniform1f(moonProgram.getLocation("uPointLights[0].linear"), 0.09f);
            glUniform1f(moonProgram.getLocation("uPointLights[0].quadratic"), 0.032f);

            glUniform3f(moonProgram.getLocation("uPointLights[1].position"), uLightPos2.x, uLightPos2.y, uLightPos2.z);
            glUniform3f(moonProgram.getLocation("uPointLights[1].ambient"), 0, 0, 0.1);
            glUniform3f(moonProgram.getLocation("uPointLights[1].diffuse"), 0, 0, 0.6);
            glUniform3f(moonProgram.getLocation("uPointLights[1].specular"), 0, 0, 0.3);
            glUniform1f(moonProgram.getLocation("uPointLights[0].constant"), 1.0f);
            glUniform1f(moonProgram.getLocation("uPointLights[0].linear"), 0.09f);
            glUniform1f(moonProgram.getLocation("uPointLights[0].quadratic"), 0.032f);
            
                
            glUniform3f(moonProgram.getLocation("uViewPos"), freeflyCamera.m_Position.x, freeflyCamera.m_Position.y, freeflyCamera.m_Position.z);
            glUniform3f(moonProgram.getLocation("material.ambient"), 0.4, 0.4, 0.4);
            glUniform3f(moonProgram.getLocation("material.diffuse"), 0.5, 0.5, 0.5);
            glUniform3f(moonProgram.getLocation("material.specular"), 0.8, 0.8, 0.8);
            glUniform1f(moonProgram.getLocation("material.shininess"), 10.0);
            

            glUniform1i(moonProgram.getLocation("uTextures[0]"), 0);

            glm::mat4 moonMMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0)); // Translation
            moonMMatrix = glm::rotate(moonMMatrix, glimac::getTime(), glm::normalize(random[i])); // Translation * Rotation
            moonMMatrix = glm::translate(moonMMatrix, glm::vec3(randomP[i], randomP[i+1], randomP[i+2])); // Translation * Rotation * Translation
            moonMMatrix = glm::scale(moonMMatrix, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
            NormalMatrix = glm::transpose(glm::inverse(VMatrix * moonMMatrix));
            glUniformMatrix4fv(moonProgram.getLocation("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(ProjMatrix * VMatrix * moonMMatrix));
            glUniformMatrix4fv(moonProgram.getLocation("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(VMatrix * moonMMatrix));
            glUniformMatrix4fv(moonProgram.getLocation("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

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
    
    stop_key_loop = true;
    thread_key.join();

    glDeleteTextures(1, &tEarth);
    glDeleteTextures(1, &tMoon);
    glDeleteTextures(1, &tCloud);
    glfwTerminate();
    return 0;
}