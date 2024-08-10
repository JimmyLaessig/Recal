#include "Common.glsl"

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec3 inWorldNormal;
layout(location = 2) in vec3 inWorldTangent;
layout(location = 3) in vec2 inTexcoords;

layout(location = 0) out vec4 outBaseColor;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outOcclusionRoughnessMetallic;

struct FragmentShaderInput
{
	vec3 worldPosition;
	vec3 worldNormal;
	vec3 worldTangent;
	vec2 texcoords;
};


struct FragmentShaderOutput
{
	vec3 baseColor;
	vec3 worldNormal;
	float occlusion;
	float roughness;
	float metallic;
};


void fragmentShader(in FragmentShaderInput shaderInput, inout FragmentShaderOutput shaderOutput);

void main()
{
	FragmentShaderInput shaderInput;

	shaderInput.worldPosition	= inWorldPosition;
	shaderInput.worldTangent	= normalize(inWorldNormal);
	shaderInput.worldTangent	= normalize(inWorldTangent);
	shaderInput.texcoords 		= inTexcoords;

	FragmentShaderOutput shaderOutput;
	shaderOutput.baseColor		= vec3(0.f,0.f,0.f);
	shaderOutput.worldNormal	= inWorldNormal;
	shaderOutput.occlusion		= 0.f;
	shaderOutput.roughness		= 1.f;
	shaderOutput.metallic		= 0.5f;
	
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