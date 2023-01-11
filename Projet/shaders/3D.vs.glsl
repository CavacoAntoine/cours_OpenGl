#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 2) in vec2 aVertexTexCoords;
layout(location = 3) in vec3 aVertexNormal;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec2 vFragTexCoords;
out vec3 vFragPosition_vs;
out vec3 vFragNormal_vs;

void main() {
  // Passage en coordonnées homogènes
  vec4 vertexPosition = vec4(aVertexPosition, 1);
  vec4 vertexNormal = vec4(aVertexNormal, 0);

  // Calcul des valeurs de sortie
  vFragPosition_vs = vec3(uMVMatrix * vertexPosition);
  vFragNormal_vs = vec3(uNormalMatrix * vertexNormal);
  vFragTexCoords = aVertexTexCoords;

  // Calcul de la position projetée
  gl_Position = uMVPMatrix * vertexPosition;
};