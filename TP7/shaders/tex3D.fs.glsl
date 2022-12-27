#version 330 core

in vec2 vFragTexCoords;
in vec3 vFragPosition_vs;
in vec3 vFragNormal_vs;

uniform sampler2D uTexture;

out vec3 fFragColor;

void main() {
  fFragColor = texture(uTexture, vFragTexCoords).xyz;
};