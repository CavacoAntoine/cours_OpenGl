#version 330 core

#define MAX_LIGHTS 10
#define MAX_TEXTURES 2

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

in vec2 vFragTexCoords;
in vec3 vFragPosition_vs;
in vec3 vFragNormal_vs;

uniform sampler2D uTextures[MAX_TEXTURES];
uniform int uNbrTextures;

uniform vec3 uViewPos;
uniform Material material;

uniform DirLight uDirLight;
uniform PointLight uPointLights[MAX_LIGHTS];
uniform int uNbrPointLights;

out vec3 fFragColor;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // combine results
    vec3 ambient  = light.ambient  * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    
    // combine results
    vec3 ambient  = light.ambient  * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
    // properties
    vec3 norm = normalize(vFragNormal_vs);
    vec3 viewDir = normalize(uViewPos - vFragPosition_vs);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(uDirLight, norm, viewDir);
    
    // phase 2: Point lights
    for(int i = 0; i < uNbrPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, vFragPosition_vs, viewDir);      

    for(int i = 0; i < uNbrTextures) {
        result *= texture(uTextures[i], vFragTexCoords).xyz;
    }

    fFragColor = result;
}
