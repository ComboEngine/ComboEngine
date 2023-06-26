#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec4 inNormal;

layout(location = 0) out vec2 texCoord;
layout(location = 1) out vec4 normal;

void main() {
    gl_Position = vec4(inPosition, 1.0);
    texCoord = inTexCoord;
    normal = inNormal;
}