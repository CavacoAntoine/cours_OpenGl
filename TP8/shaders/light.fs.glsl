#version 330 core

in vec2 vFragTexCoords;
in vec3 vFragPosition_vs;
in vec3 vFragNormal_vs;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

uniform sampler2D uTexture;

out vec3 fFragColor;

vec3 blinnPhong() {
  // wi => uLightDir_vs normalize
  // Li => uLightIntensity;
  // dot pour produit scalaire et pow pour la puissance
  // m1 = Kd(wi * N)
  // m2 = ks * (half * N)^shine
  // Li( m1 + m2)
  vec3 wi = normalize(uLightDir_vs);
  vec3 wo = normalize(-vFragPosition_vs);
  vec3 halfVector = (wo + wi) / 2;
  vec3 m1 = uKd * dot(wi, vFragNormal_vs);
  vec3 m2 = uKs * pow(dot(halfVector, vFragNormal_vs), uShininess);
  return uLightIntensity * (m1 + m2);
}

void main() {
    float ambiantStrength = 0;
    vec3 ambiant = ambiantStrength * uLightIntensity;
    vec3 norm = normalize(vFragNormal_vs);
    vec3 lightDir = normalize(uLightPos_vs - vFragPosition_vs);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightIntensity;
    fFragColor = (ambiant + diffuse) * texture(uTexture, vFragTexCoords).xyz;
};