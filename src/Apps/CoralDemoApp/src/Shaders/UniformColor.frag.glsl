#version 460
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec2 inTexcoords;

layout(std140, set = 0, binding = 1) uniform LightData
{
	vec3 color;
	vec3 direction;
} lightData;

layout(set = 0, binding = 2) uniform sampler2D colorTexture;

layout(location = 0) out vec4 outColor;

void main()
{
	vec3 color	 = lightData.color * dot(normalize(inNormal), normalize(lightData.direction));
	outColor.rgb = texture(colorTexture, inTexcoords).rgb * color;
	outColor.a	 = 1;
}