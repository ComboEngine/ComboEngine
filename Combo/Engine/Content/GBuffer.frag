#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec4 normal;

void main() {
    outColor = vec4(1.0,1.0,1.0,1.0);
}