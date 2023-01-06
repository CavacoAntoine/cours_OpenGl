#include <glad/glad.h>
#include "common.hpp"
#include "Program.hpp"
#include "FilePath.hpp"
#include <map>

class EditProgram {

    private:
        std::map<std::string, GLuint> uLocation; 

    public:
        glimac::Program m_Program;

        EditProgram(const glimac::FilePath& applicationPath, std::string fpVS, std::string fpFS):
            m_Program(loadProgram(applicationPath.dirPath() + fpVS, applicationPath.dirPath() + fpFS)) {
            this->adduLocation("uMVPMatrix");
            this->adduLocation("uMVMatrix");
            this->adduLocation("uNormalMatrix");
            this->adduLocation("uViewPos");
            this->adduLocation("material.ambient");
            this->adduLocation("material.diffuse");
            this->adduLocation("material.specular");
            this->adduLocation("material.shininess");
        }

        void adduLocation(std::string name);
        GLuint getLocation(std::string);
};

class EarthProgram : public EditProgram {
    public:
        EarthProgram(const glimac::FilePath& applicationPath, std::string fpVS, std::string fpFS) : EditProgram(applicationPath, fpVS, fpFS) {            
            this->adduLocation("uTexture1");
            this->adduLocation("uTexture2");
            this->adduLocation("uLightColor");
            this->adduLocation("uLightPos");
            this->adduLocation("uLightDir");
        }
};

class MoonProgram : public EditProgram {
    public:
        MoonProgram(const glimac::FilePath& applicationPath, std::string fpVS, std::string fpFS) : EditProgram(applicationPath, fpVS, fpFS) {            
            this->adduLocation("uTexture");
            this->adduLocation("uLightColor");
            this->adduLocation("uLightPos");
            this->adduLocation("uLightDir");
        }
};

class LightsTextsProgram : public EditProgram {
    public:
        LightsTextsProgram(const glimac::FilePath& applicationPath, std::string fpVS, std::string fpFS) : EditProgram(applicationPath, fpVS, fpFS) {            
            this->adduLocation("uTexture");
            this->adduLocation("uLightColor");
            this->adduLocation("uLightPos");
            this->adduLocation("uLightDir");
        }
};