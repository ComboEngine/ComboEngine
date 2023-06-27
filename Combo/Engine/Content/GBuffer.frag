#version 450

layout(location = 0) out vec4 positionColor;
layout(location = 1) out vec4 diffuseColor;
layout(location = 2) out vec4 normalColor;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 normal;

void main() {
    positionColor = vec4(position,1.0);
    diffuseColor = vec4(texCoord,1,1);
    normalColor = vec4(normal.xyz,1);
}