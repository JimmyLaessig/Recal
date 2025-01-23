#ifndef MATERIALBASE_FRAGMENT_GLSL
#define MATERIALBASE_FRAGMENT_GLSL

#include "Common.glsl"
#include "DefaultUniforms.h"

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec3 inWorldNormal;
layout(location = 2) in vec3 inWorldTangent;
layout(location = 3) in vec2 inTexcoord0;

layout(location = 0) out vec4 outBaseColor;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outOcclusionRoughnessMetallic;



struct FragmentShaderInput
{
	vec3 worldPosition;
	vec3 worldNormal;
	vec3 worldTangent;
	vec2 texcoord0;
};


struct FragmentShaderOutput
{
	vec3 baseColor;
	vec3 worldNormal;
	float occlusion;
	float roughness;
	float metallic;
	float opacity;
	float opacityMask;
};


void fragmentShader(in FragmentShaderInput shaderInput, inout FragmentShaderOutput shaderOutput);

void main()
{
	FragmentShaderInput shaderInput;

	shaderInput.worldPosition = inWorldPosition;
	shaderInput.worldNormal	  = normalize(inWorldNormal);
	shaderInput.worldTangent  = normalize(inWorldTangent);
	shaderInput.texcoord0 	  = inTexcoord0;

	FragmentShaderOutput shaderOutput;
	shaderOutput.baseColor		= vec3(1.f, 1.f, 1.f);
	shaderOutput.worldNormal	= inWorldNormal;
	shaderOutput.occlusion		= 0.f;
	shaderOutput.roughness		= 1.f;
	shaderOutput.metallic		= 0.5f;
	shaderOutput.opacity		= 1.f;
	shaderOutput.opacityMask	= 1.f;

	fragmentShader(shaderInput, shaderOutput);

	outBaseColor.rgb = shaderOutput.baseColor;
	outBaseColor.a	= 1.f;

	outNormal.rgb	= shaderOutput.worldNormal;
	outNormal.a		= 1.f;

	outOcclusionRoughnessMetallic.r = shaderOutput.occlusion;
	outOcclusionRoughnessMetallic.g = shaderOutput.roughness;
	outOcclusionRoughnessMetallic.g = shaderOutput.metallic;
	outOcclusionRoughnessMetallic.a = 1.f;
}

#endif // MATERIALBASE_FRAGMENT_GLSL
