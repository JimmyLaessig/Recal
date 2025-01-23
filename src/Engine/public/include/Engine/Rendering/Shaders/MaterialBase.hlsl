#ifndef MATERIALBASE_HLSL
#define MATERIALBASE_HLSL

//#include "Common.glsl"
#include "DefaultUniforms.hlsl"

const int BLENDMODE_OPAQUE 		= 1;
const int BLENDMODE_TRANSPARENT = 2;
const int BLENDMODE_MASKED 		= 3;

#define BlendMode BLENDMODE_OPAQUE;

/// Input struct of the Vertex shader stage
struct VSInput
{
	[[vk::location(0)]] float3 Position : POSITION;
	[[vk::location(1)]] float3 Normal : NORMAL;
	[[vk::location(2)]] float3 Tangent : TANGENT;
	[[vk::location(3)]] float3 Texcoord0 : TEXCOORD0;
};

/// Output struct of the Vertex shader stage
struct VSOutput
{
	float4 Position : SV_POSITION;
	float3 WorldPosition : POSITION;
	float3 WorldNormal : NORMAL;
	float3 WorldTangent: TANGENT;
	float2 Texcoord0: TEXCOORD0;
};

/// Input struct of the Pixel shader stage
struct PSInput
{
	float4 Position : SV_POSITION;
	float3 WorldPosition : POSITION;
	float3 WorldNormal : NORMAL;
	float3 WorldTangent: TANGENT;
	float2 Texcoord0: TEXCOORD0;
};

/// Output struct of the Pixel shader stage
struct PSOutput
{
	[[vk::location(0)]] float4 BaseColor: SV_Target;
};

/// Input of the Vertex shader material function
struct VertexShaderMaterialFunctionInput
{
	float3 WorldPosition;
	float3 WorldNormal;
	float3 WorldTangent;
	float2 Texcoord0;
};


/// Output of the Vertex shader material function
struct VertexShaderMaterialFunctionOutput
{
	float3 WorldPosition;
	float3 WorldNormal;
	float3 WorldTangent;
	float2 Texcoord0;
};


void vertexShaderMaterialFunction(in VertexShaderMaterialFunctionInput input, inout VertexShaderMaterialFunctionOutput output);


struct PixelShaderMaterialFunctionInput
{
	float3 Position;
	float3 Normal;
	float3 Tangent;
	float3 WorldPosition;
	float3 WorldNormal;
	float3 WorldTangent;
	float2 Texcoord0;
};


struct PixelShaderMaterialFunctionOutput
{
	float3 WorldNormal;
	float3 BaseColor;
	float opacity;
	float occlusion;
	float roughness;
	float metallic;
	float opacityThreshold;
};


void pixelShaderMaterialFunction(in PixelShaderMaterialFunctionInput input, inout PixelShaderMaterialFunctionOutput output);


VSOutput VS_main(VSInput vsInput, uint VertexIndex: SV_VERTEXID)
{
	float4 wp = mul(modelMatrix, float4(vsInput.Position, 1.f));
	wp = wp / wp.w;

	VertexShaderMaterialFunctionInput input;
	input.WorldPosition = wp.xyz;
	input.WorldNormal	= normalize(mul(normalMatrix, vsInput.Normal));
	input.WorldTangent  = normalize(mul(normalMatrix, vsInput.Tangent));
	input.Texcoord0	    = vsInput.Texcoord0;

	VertexShaderMaterialFunctionOutput output;
	output.WorldPosition   = input.WorldPosition;
	output.WorldNormal	   = input.WorldNormal;
	output.WorldTangent    = input.WorldTangent;
	output.Texcoord0	   = input.Texcoord0;

	vertexShaderMaterialFunction(input, output);

	VSOutput result;
	result.Position      = mul(viewProjectionMatrix, float4(output.WorldPosition, 1.f));
	result.WorldPosition = output.WorldPosition;
	result.WorldNormal   = output.WorldNormal;
	result.WorldTangent  = output.WorldTangent;
	result.Texcoord0     = output.Texcoord0;

	return result;
}


PSOutput PS_main(PSInput psInput)
{
	PixelShaderMaterialFunctionInput input;
	input.WorldPosition = psInput.WorldPosition;
	input.WorldNormal   = psInput.WorldNormal;
	input.WorldTangent  = psInput.WorldTangent; 
	input.Texcoord0	    = psInput.Texcoord0;

	PixelShaderMaterialFunctionOutput output;
	output.BaseColor 	    = float3(0.f, 0.f, 0.f);
	output.WorldNormal      = psInput.WorldNormal;
	output.opacity          = 1.f;
	output.occlusion        = 1.f;
	output.roughness        = 1.f;
	output.metallic         = 0.5f;
	output.opacityThreshold = 0.5f;

	pixelShaderMaterialFunction(input, output);

	PSOutput result;
	result.BaseColor.rgb = output.BaseColor;
	result.BaseColor.a = output.opacity;
	return result;
}

#endif // !MATERIALBASE_HLSL
