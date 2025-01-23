#ifndef MATERIALBASE_VERTEX_GLSL
#define MATERIALBASE_VERTEX_GLSL

#include "DefaultUniforms.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
//layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexcoord0;

layout(location = 0) out vec3 outWorldPosition;
layout(location = 1) out vec3 outWorldNormal;
layout(location = 2) out vec3 outWorldTangent;
layout(location = 3) out vec2 outTexcoord0;


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
	vec3 worldNormal;
	vec3 worldTangent;
	vec2 texcoord0;
};


void vertexShader(in VertexShaderInput shaderInput, inout VertexShaderOutput shaderOutput);


void main()
{
	vec4 wp = modelMatrix * vec4(inPosition, 1.f);
	wp = wp / wp.w;
	VertexShaderInput shaderInput;
	shaderInput.position 	= inPosition;
	shaderInput.normal 		= inNormal;
	shaderInput.tangent 	= vec3(0, 0, 0);	// inTangent;
	shaderInput.texcoord0	= inTexcoord0;

	VertexShaderOutput shaderOutput;
	shaderOutput.worldPosition   = wp.xyz;
	shaderOutput.worldNormal	 = normalize(normalMatrix * shaderInput.normal);
	shaderOutput.worldTangent    = normalize(normalMatrix * shaderInput.tangent);
	shaderOutput.texcoord0	     = shaderInput.texcoord0;

	vertexShader(shaderInput, shaderOutput);

	gl_Position		 = viewProjectionMatrix * vec4(shaderOutput.worldPosition, 1.f);

	outWorldPosition = shaderOutput.worldPosition;
	outWorldNormal	 = shaderOutput.worldNormal;
	outWorldTangent  = shaderOutput.worldTangent;
	outTexcoord0	 = shaderOutput.texcoord0;
}

#endif // MATERIALBASE_VERTEX_GLSL