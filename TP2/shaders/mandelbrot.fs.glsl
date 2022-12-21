#version 330 core

in vec3 vFragColor;
in vec2 vVertexPosition;

out vec3 fFragColor;

void main() {
  fFragColor = vec3(vVertexPosition.x, vVertexPosition.y ,0);
};