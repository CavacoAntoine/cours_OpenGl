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
        }

        void adduLocation(std::string name);
        GLuint getLocation(std::string);
};

class EarthProgram : public EditProgram {
    public:
        EarthProgram(const glimac::FilePath& applicationPath, std::string fpVS, std::string fpFS) : EditProgram(applicationPath, fpVS, fpFS) {            
            this->adduLocation("uTexture1");
            this->adduLocation("uTexture2");
        }
};

class MoonProgram : public EditProgram {
    public:
        MoonProgram(const glimac::FilePath& applicationPath, std::string fpVS, std::string fpFS) : EditProgram(applicationPath, fpVS, fpFS) {            
            this->adduLocation("uTexture");
        }
};

class DirectionalLightProgram : public EditProgram {
    public:
        DirectionalLightProgram(const glimac::FilePath& applicationPath, std::string fpVS, std::string fpFS) : EditProgram(applicationPath, fpVS, fpFS) {            
            this->adduLocation("uTexture");
            this->adduLocation("uKd");
            this->adduLocation("uKs");
            this->adduLocation("uShininess");
            this->adduLocation("uLightDir_vs");
            this->adduLocation("uLightIntensity");
        }
};

class PonctualLightProgram : public EditProgram {
    public:
        PonctualLightProgram(const glimac::FilePath& applicationPath, std::string fpVS, std::string fpFS) : EditProgram(applicationPath, fpVS, fpFS) {            
            this->adduLocation("uTexture");
            this->adduLocation("uKd");
            this->adduLocation("uKs");
            this->adduLocation("uShininess");
            this->adduLocation("uLightPos_vs");
            this->adduLocation("uLightIntensity");
        }
};