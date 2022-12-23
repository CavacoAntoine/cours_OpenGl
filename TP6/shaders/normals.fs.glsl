#version 330 core

in vec2 vFragTexCoords;
in vec3 vFragPosition_vs;
in vec3 vFragNormal_vs;

out vec3 fFragColor;

void main() {
  fFragColor = normalize(vFragNormal_vs);
};