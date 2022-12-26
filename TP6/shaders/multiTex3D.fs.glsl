#version 330 core

in vec2 vFragTexCoords;
in vec3 vFragPosition_vs;
in vec3 vFragNormal_vs;

uniform sampler2D uTexture;
uniform sampler2D uTexture2;

out vec3 fFragColor;

void main() {
  fFragColor = texture(uTexture, vFragTexCoords).xyz + texture(uTexture2, vFragTexCoords).xyz;
};