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

struct SpotLight {
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;

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

uniform vec3 uViewPos;
uniform Material material;

uniform bool uIsDirLight = false;
uniform DirLight uDirLight;
uniform PointLight uPointLights[MAX_LIGHTS];
uniform int uNbrPointLights = 0;
uniform SpotLight uSpotLights[MAX_LIGHTS];
uniform int uNbrSpotLights = 0;


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

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
    
    if(theta > light.outerCutOff) 
    {       
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
        vec3 diffuse  = light.diffuse  * diff * material.diffuse;
        vec3 specular = light.specular * spec * material.specular;
        diffuse  *= attenuation;
        diffuse  *= intensity;
        specular *= attenuation;
        specular *= intensity;

        return (diffuse + specular);
    }

    
    return vec3(0);
    
}

void main()
{
    // properties
    vec3 norm = normalize(vFragNormal_vs);
    vec3 viewDir = normalize(uViewPos - vFragPosition_vs);

    vec3 result = vec3(0);

    // phase 1: Directional lighting
    if(uIsDirLight) {
        result = CalcDirLight(uDirLight, norm, viewDir);   
    }
    
    // phase 2: Point lights
    for(int i = 0; i < uNbrPointLights; i++) {
        result += CalcPointLight(uPointLights[i], norm, vFragPosition_vs, viewDir);      
    }
        

    // phase 3: Spot light
    for(int i = 0; i < uNbrSpotLights; i++) {
        result += CalcSpotLight(uSpotLights[i], norm, vFragPosition_vs, viewDir);
    }

    fFragColor = result * texture(uTextures[0], vFragTexCoords).xyz;
}
