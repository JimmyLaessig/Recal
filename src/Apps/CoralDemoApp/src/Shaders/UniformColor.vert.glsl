#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexcoords;

layout(set = 0, binding = 0) uniform InstanceData
{
	mat4 modelViewProjectionMatrix;
	mat3 normalMatrix;
};


layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec2 outTexcoords;

void main()
{
	outNormal = normalize(normalMatrix * inNormal);
	outTexcoords = inTexcoords.xy;
	gl_Position = modelViewProjectionMatrix * vec4(inPosition, 1);
}