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


void fragmentShader(in FragmentShaderInput input, inout FragmentShaderOutput output);

void main()
{
	FragmentShaderInput input;

	input.worldPosition = inWorldPosition;
	input.worldTangent  = normalize(inWorldNormal);
	input.worldTangent 	= normalize(inWorldTangent);
	input.texcoords 	= inTexcoords;

	FragmentShaderOutput output;
	output.baseColor	= vec3(0.f,0.f,0.f);
	output.worldNormal	= inWorldNormal;
	output.occlusion	= 0.f;
	output.roughness	= 1.f;
	output.metallic		= 0.5f;
	
	fragmentShader(input, output);

	outBaseColor.rgb = output.baseColor;
	outBaseColor.a	= 1.f;

	outNormal.rgb	= output.worldNormal;
	outNormal.a		= 1.f;

	outOcclusionRoughnessMetallic.r = output.occlusion;
	outOcclusionRoughnessMetallic.g = output.roughness;
	outOcclusionRoughnessMetallic.g = output.metallic;
	outOcclusionRoughnessMetallic.a = 1.f;
}