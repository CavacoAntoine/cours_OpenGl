#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 2) in vec2 aVertexTexture;

uniform mat3 uModelMatrix;

out vec2 vFragColor;

mat3 translate(float tx, float ty) {
  return mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(tx, ty, 1));
}

mat3 scale(float sx, float sy) {
  return mat3(vec3(sx, 0, 0), vec3(0, sy, 0), vec3(0, 0, 1));
}

mat3 rotate(float a) {
  a = radians(a);
  return mat3(vec3(cos(a), sin(a), 0), vec3(-sin(a), cos(a), 0), vec3(0, 0, 1));
}

void main() {
  vFragColor = aVertexTexture;
  gl_Position = vec4( (vec3(aVertexPosition,1) * uModelMatrix).xy, 0, 1);
};