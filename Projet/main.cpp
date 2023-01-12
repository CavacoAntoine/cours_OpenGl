#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Pad.hpp>
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
const float SPEED_TRANSLATE = 0.01;
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
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Projet", nullptr, nullptr);
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
    EditProgram backGroundProgram = LightsTextsProgram(applicationPath, "Projet/shaders/3D.vs.glsl", "Projet/shaders/LightsText.fs.glsl", true, 1, 0, 0);
    EditProgram mainProgram = LightsTextsProgram(applicationPath, "Projet/shaders/3D.vs.glsl", "Projet/shaders/LightsText.fs.glsl", true, 1, 0, 0);

    /*
    *   Init des textures
    */

    std::unique_ptr<glimac::Image> sky = glimac::loadImage(applicationPath.dirPath() + "/assets/textures/cieltest.jpg");
    if(sky == NULL){
        std::cout <<"Problème chargement image sky";
        return -1;
    }

    std::unique_ptr<glimac::Image> ground = glimac::loadImage(applicationPath.dirPath() + "/assets/textures/Ground.jpg");
    if(ground == NULL){
        std::cout <<"Problème chargement image ground";
        return -1;
    }

    std::unique_ptr<glimac::Image> alu = glimac::loadImage(applicationPath.dirPath() + "/assets/textures/rails.png");
    if(ground == NULL){
        std::cout <<"Problème chargement image alu";
        return -1;
    }

    std::unique_ptr<glimac::Image> bois = glimac::loadImage(applicationPath.dirPath() + "/assets/textures/bois.png");
    if(ground == NULL){
        std::cout <<"Problème chargement image bois";
        return -1;
    }

    GLuint tSky, tGround, tAlu, tBois;
    glGenTextures(1, &tSky);
    glBindTexture(GL_TEXTURE_2D, tSky);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sky -> getWidth(), sky -> getHeight(), 0, GL_RGBA, GL_FLOAT, sky -> getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &tGround);
    glBindTexture(GL_TEXTURE_2D, tGround);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ground -> getWidth(), ground -> getHeight(), 0, GL_RGBA, GL_FLOAT, ground -> getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    glGenTextures(1, &tAlu);
    glBindTexture(GL_TEXTURE_2D, tAlu);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, alu -> getWidth(), alu -> getHeight(), 0, GL_RGBA, GL_FLOAT, alu -> getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &tBois);
    glBindTexture(GL_TEXTURE_2D, tBois);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bois -> getWidth(), bois -> getHeight(), 0, GL_RGBA, GL_FLOAT, bois -> getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(1,0);

    /*
    *   Init des objets
    */

    glimac::Sphere sphere = glimac::Sphere(45,100,100);

    // Création du vbo
    GLuint vbo_Sphere;
    glGenBuffers(1, &vbo_Sphere);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_Sphere);
    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(glimac::ShapeVertex),(glimac::ShapeVertex *) sphere.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Création du vao
    GLuint vao_Sphere;
    glGenVertexArrays(1, &vao_Sphere);
    glBindVertexArray(vao_Sphere);
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_Sphere);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glimac::Pad background = glimac::Pad(1,100,100);

    // Création du vbo
    GLuint vbo_Back;
    glGenBuffers(1, &vbo_Back);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_Back);
    glBufferData(GL_ARRAY_BUFFER, background.getVertexCount() * sizeof(glimac::ShapeVertex),(glimac::ShapeVertex *) background.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Création du vao
    GLuint vao_Back;
    glGenVertexArrays(1, &vao_Back);
    glBindVertexArray(vao_Back);
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_Back);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Cylindre
    glimac::Cylinder cylindre = glimac::Cylinder(0.5, 0.03, 30, 30);

    // Création du vbo
    GLuint vbo_Cyl;
    glGenBuffers(1, &vbo_Cyl);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_Cyl);
    glBufferData(GL_ARRAY_BUFFER, cylindre.getVertexCount() * sizeof(glimac::ShapeVertex),(glimac::ShapeVertex *) cylindre.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Création du vao
    GLuint vao_Cyl;
    glGenVertexArrays(1, &vao_Cyl);
    glBindVertexArray(vao_Cyl);
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_Cyl);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Cylindre
    glimac::Cylinder cylindreP = glimac::Cylinder(0.52, 0.03, 30, 30);

    // Création du vbo
    GLuint vbo_CylP;
    glGenBuffers(1, &vbo_CylP);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_CylP);
    glBufferData(GL_ARRAY_BUFFER, cylindreP.getVertexCount() * sizeof(glimac::ShapeVertex),(glimac::ShapeVertex *) cylindreP.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Création du vao
    GLuint vao_CylP;
    glGenVertexArrays(1, &vao_CylP);
    glBindVertexArray(vao_CylP);
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_CylP);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex) , (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Lame de bois

    glimac::Pad lame = glimac::Pad(0.02, 0.3, 0.05);

    // Création du vbo
    GLuint vbo_Lame;
    glGenBuffers(1, &vbo_Lame);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_Lame);
    glBufferData(GL_ARRAY_BUFFER, lame.getVertexCount() * sizeof(glimac::ShapeVertex),(glimac::ShapeVertex *) lame.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Création du vao
    GLuint vao_Lame;
    glGenVertexArrays(1, &vao_Lame);
    glBindVertexArray(vao_Lame);
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_Lame);
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

    /* Dir lights */
    glm::vec4 dirLight(0.5, 0.5, 0.5, 0);

    /* Spot lights */
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.75f, 0.75f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        VMatrix = freeflyCamera.getViewMatrix();

        glm::vec4 uDirLight = VMatrix * dirLight;

        /*
        *   backGround Program
        */

        backGroundProgram.m_Program.use();

        // Sky

        glBindVertexArray(vao_Sphere);

        glUniform1i(backGroundProgram.getLocation("uIsDirLight"), 1);

        glUniform3f(backGroundProgram.getLocation("uDirLight.direction"), uDirLight.x, uDirLight.y, uDirLight.z);
        glUniform3f(backGroundProgram.getLocation("uDirLight.ambient"), 1,1,1);
        glUniform3f(backGroundProgram.getLocation("uDirLight.diffuse"), 0, 0, 0);
        glUniform3f(backGroundProgram.getLocation("uDirLight.specular"), 0, 0, 0);

        glUniform3f(backGroundProgram.getLocation("uViewPos"), freeflyCamera.m_Position.x, freeflyCamera.m_Position.y, freeflyCamera.m_Position.z);
        
        glUniform3f(backGroundProgram.getLocation("material.ambient"), 1, 1, 1);
        glUniform3f(backGroundProgram.getLocation("material.diffuse"), 0, 0, 0);
        glUniform3f(backGroundProgram.getLocation("material.specular"), 0, 0, 0);
        glUniform1f(backGroundProgram.getLocation("material.shininess"), 10.0);

        glUniform1i(backGroundProgram.getLocation("uTextures[0]"), 0);

        MMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
        NormalMatrix = glm::transpose(glm::inverse(VMatrix * MMatrix));
        glUniformMatrix4fv(backGroundProgram.getLocation("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(ProjMatrix * VMatrix * MMatrix));
        glUniformMatrix4fv(backGroundProgram.getLocation("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(VMatrix * MMatrix));
        glUniformMatrix4fv(backGroundProgram.getLocation("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tSky);

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        
        // Ground
        
        glBindVertexArray(vao_Back);

        glUniform3f(backGroundProgram.getLocation("material.ambient"), 1, 1, 1);
        glUniform3f(backGroundProgram.getLocation("material.diffuse"), 0, 0, 0);
        glUniform3f(backGroundProgram.getLocation("material.specular"), 0, 0, 0);
        glUniform1f(backGroundProgram.getLocation("material.shininess"), 10.0);

        glUniform1i(backGroundProgram.getLocation("uTextures[0]"), 0);

        MMatrix = glm::translate(glm::mat4(1), glm::vec3(-50, 0, 50));
        MMatrix = glm::rotate(MMatrix, glm::radians(180.f), glm::vec3(1,0,0));
        NormalMatrix = glm::transpose(glm::inverse(VMatrix * MMatrix));
        glUniformMatrix4fv(backGroundProgram.getLocation("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(ProjMatrix * VMatrix * MMatrix));
        glUniformMatrix4fv(backGroundProgram.getLocation("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(VMatrix * MMatrix));
        glUniformMatrix4fv(backGroundProgram.getLocation("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tGround);

        glDrawArrays(GL_TRIANGLES, 0, background.getVertexCount());

        /*
        *   MainProgram
        */

        mainProgram.m_Program.use();

        glUniform1i(mainProgram.getLocation("uIsDirLight"), 1);

        glUniform3f(mainProgram.getLocation("uDirLight.direction"), uDirLight.x, uDirLight.y, uDirLight.z);
        glUniform3f(mainProgram.getLocation("uDirLight.ambient"), 1,1,1);
        glUniform3f(mainProgram.getLocation("uDirLight.diffuse"), 0, 0, 0);
        glUniform3f(mainProgram.getLocation("uDirLight.specular"), 0, 0, 0);

        glUniform3f(mainProgram.getLocation("uViewPos"), freeflyCamera.m_Position.x, freeflyCamera.m_Position.y, freeflyCamera.m_Position.z);

        glUniform3f(mainProgram.getLocation("material.ambient"), 1, 1, 1);
        glUniform3f(mainProgram.getLocation("material.diffuse"), 0, 0, 0);
        glUniform3f(mainProgram.getLocation("material.specular"), 0, 0, 0);
        glUniform1f(mainProgram.getLocation("material.shininess"), 10.0);

        glUniform1i(mainProgram.getLocation("uTextures[0]"), 0);

        glActiveTexture(GL_TEXTURE0);

        for(int j = 0; j<10 ; j++) {
            glBindVertexArray(vao_Cyl);
            glBindTexture(GL_TEXTURE_2D, tAlu);
            MMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0.5, 0 + j * 0.483));
            NormalMatrix = glm::transpose(glm::inverse(VMatrix * MMatrix));
            glUniformMatrix4fv(mainProgram.getLocation("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(ProjMatrix * VMatrix * MMatrix));
            glUniformMatrix4fv(mainProgram.getLocation("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(VMatrix * MMatrix));
            glUniformMatrix4fv(mainProgram.getLocation("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, cylindre.getVertexCount());

            MMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.3, 0.5, 0 + j * 0.483));
            NormalMatrix = glm::transpose(glm::inverse(VMatrix * MMatrix));
            glUniformMatrix4fv(mainProgram.getLocation("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(ProjMatrix * VMatrix * MMatrix));
            glUniformMatrix4fv(mainProgram.getLocation("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(VMatrix * MMatrix));
            glUniformMatrix4fv(mainProgram.getLocation("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, cylindre.getVertexCount());

            // Lames de bois

            glBindVertexArray(vao_Lame);

            glUniform3f(mainProgram.getLocation("material.ambient"), 1, 1, 1);
            glUniform3f(mainProgram.getLocation("material.diffuse"), 0, 0, 0);
            glUniform3f(mainProgram.getLocation("material.specular"), 0, 0, 0);
            glUniform1f(mainProgram.getLocation("material.shininess"), 10.0);

            glUniform1i(mainProgram.getLocation("uTextures[0]"), 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tBois);

            for(int i = 0; i<3 ; i++) {
                MMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.3, 0.49, 0.05 + i*0.16 + j * 0.483));
                NormalMatrix = glm::transpose(glm::inverse(VMatrix * MMatrix));
                glUniformMatrix4fv(mainProgram.getLocation("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(ProjMatrix * VMatrix * MMatrix));
                glUniformMatrix4fv(mainProgram.getLocation("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(VMatrix * MMatrix));
                glUniformMatrix4fv(mainProgram.getLocation("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

                glDrawArrays(GL_TRIANGLES, 0, lame.getVertexCount());
            }
        }
        
        // Début du tournant

        glUniform3f(mainProgram.getLocation("material.ambient"), 1, 1, 1);
        glUniform3f(mainProgram.getLocation("material.diffuse"), 0, 0, 0);
        glUniform3f(mainProgram.getLocation("material.specular"), 0, 0, 0);
        glUniform1f(mainProgram.getLocation("material.shininess"), 10.0);

        glUniform1i(mainProgram.getLocation("uTextures[0]"), 0);

        glActiveTexture(GL_TEXTURE0);

        for(int i = 0; i < 3;i++){
            glBindVertexArray(vao_CylP);
            glBindTexture(GL_TEXTURE_2D, tAlu);
            float angle = glm::radians(i * (90.0f/2));
            MMatrix = glm::translate(glm::mat4(1), glm::vec3((i * glm::cos(angle)), 0.5, 4.82 + (i * glm::sin(angle))));
            MMatrix = glm::rotate(MMatrix, angle, glm::vec3(0,1,0));
            NormalMatrix = glm::transpose(glm::inverse(VMatrix * MMatrix));
            glUniformMatrix4fv(mainProgram.getLocation("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(ProjMatrix * VMatrix * MMatrix));
            glUniformMatrix4fv(mainProgram.getLocation("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(VMatrix * MMatrix));
            glUniformMatrix4fv(mainProgram.getLocation("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, cylindre.getVertexCount());
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

    glDeleteTextures(1, &tSky);
    glDeleteTextures(1, &tGround);
    glDeleteTextures(1, &tAlu);
    glDeleteTextures(1, &tBois);
    glfwTerminate();
    return 0;
}