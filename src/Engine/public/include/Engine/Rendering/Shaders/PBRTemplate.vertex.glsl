#include "Common.glsl"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexcoord0;

layout(set = 0, binding = 0) uniform PerInstanceData
{
	uniform mat4 viewMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 viewProjectionMatrix;

	uniform mat4 modelMatrix;
	uniform mat4 modelViewMatrix;
	uniform mat4 modelViewProjectionMatrix;
	uniform mat3 normalMatrix;
};


layout(location = 0) out vec3 outWorldNormal;
layout(location = 1) out vec3 outWorldTangent;
layout(location = 2) out vec2 outTexcoords;


struct VertexShaderInput
{
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 texcoord0;
};


struct VertexShaderOutput
{
	vec3 worldPosition;
};


void vertexShader(in VertexShaderInput shaderInput, inout VertexShaderOutput shaderOutput);


void main()
{
	vec4 wp = modelMatrix * vec4(inPosition, 1.f);

	VertexShaderInput shaderInput;
	shaderInput.position 	= inPosition;
	shaderInput.normal 		= inNormal;
	shaderInput.tangent 	= inTangent;
	shaderInput.texcoord0	= inTexcoord0;

	VertexShaderOutput shaderOutput;
	shaderOutput.worldPosition = wp.xyz / wp.w;

	vertexShader(shaderInput, shaderOutput);

	gl_Position		= viewProjectionMatrix * vec4(shaderOutput.worldPosition, 1.f);
	outWorldNormal	= normalMatrix * inNormal;
	outWorldTangent = normalMatrix * inTangent;
	outTexcoords	= inTexcoord0;
}