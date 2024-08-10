#include "Common.glsl"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexcoord0;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewProjectionMatrix;

uniform mat4 modelMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

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


void vertexShader(in VertexShaderInput input, inout VertexShaderOutput output);


void main()
{
	vec4 wp = modelMatrix * vec4(inPosition, 1.f);

	VertexShaderInput input;
	input.position 	= inPosition;
	input.normal 	= inNormal;
	input.tangent 	= inTangent;
	input.texcoord0	= inTexcoord0;

	VertexShaderOutput output;
	output.worldPosition = wp.xyz / wp.w;

	vertexShader(input, output);

	gl_Position		= viewProjectionMatrix * vec4(output.worldPosition, 1.f);
	outWorldNormal	= normalMatrix * inNormal;
	outWorldTangent = normalMatrix * inTangent;
	outTexcoords	= inTexcoord0;
}