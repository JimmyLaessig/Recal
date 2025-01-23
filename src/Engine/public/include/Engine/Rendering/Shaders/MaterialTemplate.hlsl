#include "MaterialBase.hlsl"


/// 
/**
 * struct VertexShaderMaterialFunctionInput
 * {
 * 	float3 WorldPosition;
 * 	float3 WorldNormal;
 * 	float3 WorldTangent;
 * 	float2 Texcoord0;
 * };
 * 
 * 
 * /// Output of the Vertex shader material function
 * struct VertexShaderMaterialFunctionOutput
 * {
 * 	float3 WorldPosition;
 * 	float3 WorldNormal;
 * 	float3 WorldTangent;
 * 	float2 Texcoord0;
 * };
 */
void vertexShaderMaterialFunction(in VertexShaderMaterialFunctionInput input, inout VertexShaderMaterialFunctionOutput output)
{

}

///
/**
 * struct PixelShaderMaterialFunctionInput
 * {
 *     float3 Position;
 * 	   float3 Normal;
 * 	   float3 Tangent;
 * 	   float3 WorldPosition;
 * 	   float3 WorldNormal;
 * 	   float3 WorldTangent;
 * 	   float2 Texcoord0;
 * };
 * 
 * 
 * struct PixelShaderMaterialFunctionOutput
 * {
 * 	   float3 WorldNormal;
 * 	   float3 BaseColor;
 * 	   float opacity;
 * 	   float occlusion;
 * 	   float roughness;
 * 	   float metallic;
 * 	   float opacityThreshold;
 * };
 */
void pixelShaderMaterialFunction(in PixelShaderMaterialFunctionInput input, inout PixelShaderMaterialFunctionOutput output)
{
	
}


