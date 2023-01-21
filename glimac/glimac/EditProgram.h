#pragma once

#include <glad/glad.h>
#include "common.hpp"
#include "Program.hpp"
#include "FilePath.hpp"
#include "Texture.hpp"
#include <map>

#define MAX_LIGHTS 10
#define MAX_TEXTURES 2

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
        GLuint getLocation(std::string name);
        void setMaterial(Material material);
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
    private:
        int nbrTextures;

    public:
        LightsTextsProgram(const glimac::FilePath& applicationPath, std::string fpVS, std::string fpFS, bool isDirLight, int nbrTextures, int nbrPointLights, int nbrSpotLights) : EditProgram(applicationPath, fpVS, fpFS), nbrTextures(nbrTextures) {
            this->m_Program.use();

            // Location of texture values
            for(int i = 0; i < nbrTextures; i++) {
                this->adduLocation(std::string("uTextures[" + std::to_string(i) + "]"));
            }

            // Location of point light values
            this->adduLocation(std::string("uNbrPointLights"));
            glUniform1i(this->getLocation("uNbrPointLights"), nbrPointLights);
            for(int i = 0; i < nbrPointLights; i++) {
                std::string base("uPointLights[" + std::to_string(i) + "]");
                this->adduLocation(std::string(base + ".position"));
                
                this->adduLocation(std::string(base + ".constant"));
                this->adduLocation(std::string(base + ".linear"));
                this->adduLocation(std::string(base + ".quadratic"));

                this->adduLocation(std::string(base + ".ambient"));
                this->adduLocation(std::string(base + ".diffuse"));
                this->adduLocation(std::string(base + ".specular"));
            }

            // Location of spot light values
            this->adduLocation(std::string("uNbrSpotLights"));
            glUniform1i(this->getLocation("uNbrSpotLights"), nbrSpotLights);
            for(int i = 0; i < nbrSpotLights; i++) {
                std::string base("uSpotLights[" + std::to_string(i) + "]");
                this->adduLocation(std::string(base + ".position"));
                this->adduLocation(std::string(base + ".direction"));
                this->adduLocation(std::string(base + ".cutOff"));
                this->adduLocation(std::string(base + ".outerCutOff"));
                

                this->adduLocation(std::string(base + ".constant"));
                this->adduLocation(std::string(base + ".linear"));
                this->adduLocation(std::string(base + ".quadratic"));

                this->adduLocation(std::string(base + ".ambient"));
                this->adduLocation(std::string(base + ".diffuse"));
                this->adduLocation(std::string(base + ".specular"));
            }

            // Location of directionnal light values
            this->adduLocation(std::string("uIsDirLight"));
            glUniform1i(this->getLocation("uIsDirLight"), isDirLight);
            if(isDirLight) {
                this->adduLocation(std::string("uDirLight.direction"));
                this->adduLocation(std::string("uDirLight.ambient"));
                this->adduLocation(std::string("uDirLight.diffuse"));
                this->adduLocation(std::string("uDirLight.specular"));
            }
            
        }

        void setMaterial(Material material);
};