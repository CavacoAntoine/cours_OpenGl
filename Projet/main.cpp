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
#include <glimac/Texture.hpp>
#include <glm/gtc/random.hpp>
#include <vector>
#include <iomanip>
#include <functional>
#include <thread>

int window_width  = 1280;
int window_height = 720;

glimac::FreeflyCamera freeflyCamera = glimac::FreeflyCamera();

const int iterationCount = 45;

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

void initMatrixs(EditProgram *program, glm::mat4 ProjMatrix, glm::mat4 VMatrix, glm::mat4 MMatrix) {
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(VMatrix * MMatrix));
    glUniformMatrix4fv(program->getLocation("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(ProjMatrix * VMatrix * MMatrix));
    glUniformMatrix4fv(program->getLocation("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(VMatrix * MMatrix));
    glUniformMatrix4fv(program->getLocation("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));
}

void setMaterial1t(EditProgram *program, Material material, GLuint textureID) {
    program->setMaterial(material);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void setMaterial2t(EditProgram *program, Material material, GLuint textureID1,GLuint textureID2) {
    program->setMaterial(material);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureID2);
}

glm::vec3 virageExterieur(EditProgram *program, glm::mat4 ProjMatrix, glm::mat4 VMatrix, glm::vec3 startPos, float angleIdx, double objectSize, 
                            GLuint vaoCyl, Material materialCyl, GLuint textureCylId, GLsizei vertexCountCyl,
                            GLuint vaoLame, Material materialLame, GLuint textureLameId, GLsizei vertexCountLame) {
    glm::vec3 position;
    glm::vec3 previousPos;
    glm::mat4 MMatrix;
    float previousAngle;
    float angle;

    for(int i = 0; i < iterationCount; i++){
        glBindVertexArray(vaoCyl);
        setMaterial1t(program, materialCyl, textureCylId);
        angle = glm::radians(i * (90.0f / (iterationCount - 1)) + angleIdx);
        if (i == 0) {
            position = startPos;
            
        } else {
            position = glm::vec3(previousPos.x + objectSize * glm::sin(previousAngle), previousPos.y, previousPos.z + objectSize * glm::cos(previousAngle));
        }

        previousAngle = angle;
        previousPos = position;

        MMatrix = glm::translate(glm::mat4(1), position);
        MMatrix = glm::rotate(MMatrix, angle, glm::vec3(0,1,0));
        initMatrixs(program, ProjMatrix, VMatrix, MMatrix);
        glDrawArrays(GL_TRIANGLES, 0, vertexCountCyl);
        
        if(i%9 == 3 && i!= 0) {
            glBindVertexArray(vaoLame);
            setMaterial1t(program, materialLame, textureLameId);
            MMatrix = glm::translate(glm::mat4(1), position);
            MMatrix = glm::rotate(MMatrix, angle , glm::vec3(0, 1, 0));
            initMatrixs(program, ProjMatrix, VMatrix, MMatrix);
            glDrawArrays(GL_TRIANGLES, 0, vertexCountLame);
        }
    }

    return position;
}

glm::vec3 virageInterieur(EditProgram *program, glm::mat4 ProjMatrix, glm::mat4 VMatrix, glm::vec3 startPos, float angleIdx, double objectSize, 
                            GLuint vao, Material material, GLuint textureId, GLsizei vertexCount) {
    glm::vec3 position;
    glm::vec3 previousPos;
    glm::mat4 MMatrix;
    float previousAngle;
    float angle;

    glBindVertexArray(vao);
    setMaterial1t(program, material, textureId);
    for(int i = 0; i < iterationCount; i++){
        angle = glm::radians(i * (90.0f / (iterationCount - 1)) + angleIdx);
        if (i == 0) {
            position = startPos;
        } else {
            position = glm::vec3(previousPos.x + objectSize * glm::sin(previousAngle), previousPos.y, previousPos.z + objectSize * glm::cos(previousAngle));
        }
        previousPos = position;
        previousAngle = angle;

        MMatrix = glm::translate(glm::mat4(1), position);
        MMatrix = glm::rotate(MMatrix, angle, glm::vec3(0,1,0));
        initMatrixs(program, ProjMatrix, VMatrix, MMatrix);        
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    return position;
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

    /* Init des textures */

    Texture sky(applicationPath.dirPath() + "/assets/textures/cieltest.jpg", Material(glm::vec3(1, 1, 1), glm::vec3(0), glm::vec3(0), 10.0f));
    Texture ground(applicationPath.dirPath() + "/assets/textures/Ground.jpg", Material(glm::vec3(1, 1, 1), glm::vec3(0), glm::vec3(0), 10.0f));
    Texture alu(applicationPath.dirPath() + "/assets/textures/rails.png", Material(glm::vec3(1, 1, 1), glm::vec3(0), glm::vec3(0), 10.0f));
    Texture bois(applicationPath.dirPath() + "/assets/textures/bois.png", Material(glm::vec3(1, 1, 1), glm::vec3(0), glm::vec3(0), 10.0f));

    GLuint tSky = sky.getID();
    GLuint tGround = ground.getID();
    GLuint tAlu = alu.getID();
    GLuint tBois = bois.getID();

    /* Init des objets */

    glimac::Sphere sphere = glimac::Sphere(45,100,100);
    GLuint vao_Sphere = sphere.getVAO();

    glimac::Pad background = glimac::Pad(1,100,100);
    GLuint vao_Back = background.getVAO();

    // Cylindre
    glimac::Cylinder cylindre = glimac::Cylinder(0.5, 0.03, 30, 30); 
    GLuint vao_Cyl = cylindre.getVAO();

    // Cylindre
    const float objectSize = 0.017;
    glimac::Cylinder cylindreP = glimac::Cylinder(objectSize, 0.03, 30, 30);
    GLuint vao_CylP = cylindreP.getVAO();

    // Cylindre
    const float objectSizeG = 0.028;
    glimac::Cylinder cylindrePG = glimac::Cylinder(objectSizeG, 0.03, 30, 30);
    GLuint vao_CylPG = cylindrePG.getVAO();

    // Lame de bois
    glimac::Pad lame = glimac::Pad(0.02, 0.3, 0.05);
    GLuint vao_Lame = lame.getVAO();

    // wagon
    glimac::Pad wagon = glimac::Pad(0.15, 0.35, 0.3);
    GLuint vao_wagon = wagon.getVAO();
    double wagonTranslation = 0;
    int direction = 1;

    glEnable(GL_DEPTH_TEST); // Permet l'activation de profondeur du GPU

    glm::mat4 ProjMatrix = glm::perspective(glm::radians(50.f), (float)window_width/(float)window_height, 0.1f, 100.f);
    glm::mat4 VMatrix;
    glm::mat4 MMatrix;

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

        /* backGround Program */
        backGroundProgram.m_Program.use();
        glUniform1i(backGroundProgram.getLocation("uIsDirLight"), 1);
        glUniform3f(backGroundProgram.getLocation("uDirLight.direction"), uDirLight.x, uDirLight.y, uDirLight.z);
        glUniform3f(backGroundProgram.getLocation("uDirLight.ambient"), 1,1,1);
        glUniform3f(backGroundProgram.getLocation("uDirLight.diffuse"), 0, 0, 0);
        glUniform3f(backGroundProgram.getLocation("uDirLight.specular"), 0, 0, 0);
        glUniform3f(backGroundProgram.getLocation("uViewPos"), freeflyCamera.m_Position.x, freeflyCamera.m_Position.y, freeflyCamera.m_Position.z);
        
        // Sky
        glBindVertexArray(vao_Sphere);
        setMaterial1t(&backGroundProgram, sky.getMaterial(), tSky);

        MMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
        initMatrixs(&backGroundProgram, ProjMatrix, VMatrix, MMatrix);

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        
        // Ground
        glBindVertexArray(vao_Back);
        setMaterial1t(&backGroundProgram, ground.getMaterial(), tGround);

        MMatrix = glm::translate(glm::mat4(1), glm::vec3(-50, 0, 50));
        MMatrix = glm::rotate(MMatrix, glm::radians(180.f), glm::vec3(1,0,0));
        initMatrixs(&backGroundProgram, ProjMatrix, VMatrix, MMatrix);

        glDrawArrays(GL_TRIANGLES, 0, background.getVertexCount());

        /* MainProgram */

        mainProgram.m_Program.use();
        glUniform1i(mainProgram.getLocation("uIsDirLight"), 1);
        glUniform3f(mainProgram.getLocation("uDirLight.direction"), uDirLight.x, uDirLight.y, uDirLight.z);
        glUniform3f(mainProgram.getLocation("uDirLight.ambient"), 1,1,1);
        glUniform3f(mainProgram.getLocation("uDirLight.diffuse"), 0, 0, 0);
        glUniform3f(mainProgram.getLocation("uDirLight.specular"), 0, 0, 0);
        glUniform3f(mainProgram.getLocation("uViewPos"), freeflyCamera.m_Position.x, freeflyCamera.m_Position.y, freeflyCamera.m_Position.z);

        // 1er rail droit

        for(int j = 0; j<10 ; j++) {
            glBindVertexArray(vao_Cyl);
            setMaterial1t(&mainProgram, alu.getMaterial(), tAlu);

            // Rail gauche
            MMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0.5, 0 + j * 0.483));
            initMatrixs(&mainProgram, ProjMatrix, VMatrix, MMatrix);
            glDrawArrays(GL_TRIANGLES, 0, cylindre.getVertexCount());

            // Rail droit
            MMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.3, 0.5, 0 + j * 0.483));
            initMatrixs(&mainProgram, ProjMatrix, VMatrix, MMatrix);
            glDrawArrays(GL_TRIANGLES, 0, cylindre.getVertexCount());

            // Lames de bois
            glBindVertexArray(vao_Lame);
            setMaterial1t(&mainProgram, bois.getMaterial(), tBois);
            for(int i = 0; i<3 ; i++) {
                MMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.3, 0.49, 0.05 + i*0.16 + j * 0.483));
                initMatrixs(&mainProgram, ProjMatrix, VMatrix, MMatrix);
                glDrawArrays(GL_TRIANGLES, 0, lame.getVertexCount());
            }
        }

        // Wagon
        glBindVertexArray(vao_wagon);
        setMaterial1t(&mainProgram, alu.getMaterial(), tAlu);
        glm::vec3 initPosWagon = glm::vec3(-0.325, 0.53, 0);
        
        MMatrix = glm::translate(glm::mat4(1), glm::vec3(initPosWagon.x , initPosWagon.y, initPosWagon.z + wagonTranslation));
        initMatrixs(&mainProgram, ProjMatrix, VMatrix, MMatrix);
        glDrawArrays(GL_TRIANGLES, 0, wagon.getVertexCount());

        wagonTranslation += 0.01 * direction;
        if(wagonTranslation >= 4.8) {
            direction = -1;   
        } if(wagonTranslation <= 0) {
            direction = 1;
        }
        
        // 1er Virage
        glm::vec3 lastPos;
        lastPos = virageInterieur(&mainProgram, ProjMatrix, VMatrix, glm::vec3(0, 0.5, 4.82), 0.0f, objectSize, vao_CylP, alu.getMaterial(), tAlu, cylindreP.getVertexCount());
        lastPos = virageInterieur(&mainProgram, ProjMatrix, VMatrix, lastPos, 90.0f,objectSize, vao_CylP, alu.getMaterial(), tAlu, cylindreP.getVertexCount());

        lastPos = virageExterieur(&mainProgram, ProjMatrix, VMatrix, glm::vec3(-0.3, 0.5, 4.82), 0.0f, objectSizeG, vao_CylPG, alu.getMaterial(), tAlu, cylindrePG.getVertexCount(),
                                    vao_Lame, bois.getMaterial(), tBois, lame.getVertexCount());
        lastPos = virageExterieur(&mainProgram, ProjMatrix, VMatrix, lastPos, 90.0f, objectSizeG, vao_CylPG, alu.getMaterial(), tAlu, cylindrePG.getVertexCount(),
                                    vao_Lame, bois.getMaterial(), tBois, lame.getVertexCount());

        // 2eme rail droit
        for(int j = 0; j<10 ; j++) {
            glBindVertexArray(vao_Cyl);
            setMaterial1t(&mainProgram, alu.getMaterial(), tAlu);
            
            MMatrix = glm::translate(glm::mat4(1), glm::vec3(lastPos.x, 0.5, 0 + j * 0.483));
            initMatrixs(&mainProgram, ProjMatrix, VMatrix, MMatrix);
            glDrawArrays(GL_TRIANGLES, 0, cylindre.getVertexCount());

            MMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.3+lastPos.x, 0.5, 0 + j * 0.483));
            initMatrixs(&mainProgram, ProjMatrix, VMatrix, MMatrix);
            glDrawArrays(GL_TRIANGLES, 0, cylindre.getVertexCount());

            // Lames de bois
            glBindVertexArray(vao_Lame);
            setMaterial1t(&mainProgram, bois.getMaterial(), tBois);

            for(int i = 0; i<3 ; i++) {
                MMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.3+lastPos.x, 0.49, 0.05 + i*0.16 + j * 0.483));
                initMatrixs(&mainProgram, ProjMatrix, VMatrix, MMatrix);
                glDrawArrays(GL_TRIANGLES, 0, lame.getVertexCount());
            }
        }

        // 2eme Virage
        lastPos = virageInterieur(&mainProgram, ProjMatrix, VMatrix, glm::vec3(-0.3+lastPos.x, 0.5, 0), 180.0f, objectSize, vao_CylP, alu.getMaterial(), tAlu, cylindreP.getVertexCount());
        lastPos = virageInterieur(&mainProgram, ProjMatrix, VMatrix, lastPos, 270.0f,objectSize, vao_CylP, alu.getMaterial(), tAlu, cylindreP.getVertexCount());

        lastPos = virageExterieur(&mainProgram, ProjMatrix, VMatrix, glm::vec3(-0.3*lastPos.x, 0.5, 0), 180.0f, objectSizeG, vao_CylPG, alu.getMaterial(), tAlu, cylindrePG.getVertexCount(),
                                    vao_Lame, bois.getMaterial(), tBois, lame.getVertexCount());
        lastPos = virageExterieur(&mainProgram, ProjMatrix, VMatrix, lastPos, 270.0f, objectSizeG, vao_CylPG, alu.getMaterial(), tAlu, cylindrePG.getVertexCount(),
                                    vao_Lame, bois.getMaterial(), tBois, lame.getVertexCount());

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