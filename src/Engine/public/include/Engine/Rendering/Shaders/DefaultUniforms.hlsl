#ifndef SHADERS_DEFAULTUNIFORMS_H
#define SHADERS_DEFAULTUNIFORMS_H

/**
 * This file contains the pre-defined uniforms used for each material
 **/

/// The descriptor set used for the per-frame uniform data
#define PERFRAMEDATA_DESCRIPTORET 0
/// The descriptor binding used for the per-frame uniform data
#define PERFRAMEDATA_DESCRIPTORBINDING 0
/// The descriptor set used for the per-instance uniform data
#define PERINSTANCEDATA_DESCRIPTORSET 1
/// The descriptor binding used for the per-instance uniform data
#define PERINSTANCEDATA_DESCRIPTORBINDING 0
/// The descriptor set used for material uniforms
#define PERMATERIAL_DESCRIPTORSET 2

#ifndef __cplusplus


[[vk::binding(0, 0)]] cbuffer PerFrameData {
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4x4 viewProjectionMatrix;
};


[[vk::binding(1, 0)]] cbuffer PerInstanceData {
	float4x4 modelMatrix;
	float4x4 inverseModelMatrix;
	float3x3 normalMatrix;
};


/// Uniform binding for per-frame data
//layout(set = PERFRAMEDATA_DESCRIPTORET, binding = PERFRAMEDATA_DESCRIPTORBINDING) uniform PerFrameData
//{
//	/// Matrix to transform a vertex from world space to view space
//	uniform mat4 viewMatrix;
//	/// Matrix to transform a vertex from view space to camera space
//	uniform mat4 projectionMatrix;
//	/// Matrix to transform a vertex from world space to camera space
//	uniform mat4 viewProjectionMatrix;
//};

/// Uniform binding for per-instance data
//layout(set = PERINSTANCEDATA_DESCRIPTORSET, binding = PERINSTANCEDATA_DESCRIPTORBINDING) uniform PerInstanceData
//{
//	/// Matrix to transform a vertex from local space to world space
//	uniform mat4 modelMatrix;
//	/// Matrix to transform a vertex from world space to local space
//	uniform mat4 inverseModelMatrix;
//	/// Matrix to transform a normal from lccal space to world space
//	uniform mat3 normalMatrix;
//};

#endif // !__cplusplus
#endif // !SHADERS_DEFAULTUNIFORMS_H
