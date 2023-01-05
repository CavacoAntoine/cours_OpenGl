#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec2 vFragTexCoords;
in vec3 vFragPosition_vs;
in vec3 vFragNormal_vs;

uniform sampler2D uTexture;
uniform sampler2D uTexture2;
uniform vec3 uLightColor;
uniform vec3 uLightPos;
uniform vec3 uLightDir;
uniform vec3 uViewPos;
uniform Material material;

out vec3 fFragColor;

void main()
{

    vec3 lightDir = normalize(-uLightDir);

    // ambient
    vec3 ambient = material.ambient * uLightColor;

    // diffuse 
    vec3 norm = normalize(vFragNormal_vs);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = uLightColor * (diff * material.diffuse);

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - vFragPosition_vs);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = uLightColor * (spec * material.specular); 

    fFragColor = (ambient + diffuse + specular) * (texture(uTexture, vFragTexCoords).xyz + texture(uTexture2, vFragTexCoords).xyz);
}
