#version 330 core

in vec2 vFragTexCoords;
in vec3 vFragPosition_vs;
in vec3 vFragNormal_vs;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

uniform sampler2D uTexture;

out vec3 fFragColor;

vec3 blinnPhong() {
  // wi => uLightDir_vs normalize
  // Li => uLightIntensity;
  // dot pour produit scalaire et pow pour la puissance
  vec3 wo = normalize(-vFragPosition_vs);
  return dot(uLightIntensity, uKd * (dot(uLightDir_vs, vFragNormal_vs)) + dot(uKs, pow(dot((wo + uLightDir_vs)/2), vFragNormal_vs), uShininess));
}

void main() {
  fFragColor = blinnPhong();
};