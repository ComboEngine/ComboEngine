#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec4 inNormal;

layout(location = 0) out vec2 texCoord;
layout(location = 1) out vec4 normal;

layout( push_constant ) uniform constants
{
        mat4 WVP;
	mat4 Model;
	mat4 RotationMatrix;
	vec4 Diffuse;
	vec4 DiffuseUseTexture;
} PushConstants;


void main() {
    gl_Position = vec4(inPosition, 1.0) * PushConstants.Model * PushConstants.WVP;
    texCoord = inTexCoord;
    normal = inNormal;
}