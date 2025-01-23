#include <Engine/Core/Shader.hpp>

#include <glm/glm.hpp>

#include <cassert>
#include <cstdint>
#include <vector>
#include <span>
#include <array>
#include <memory>
#include <string>

using namespace Reef;


std::span<const std::byte>
Shader::vertexShaderSource() const
{
	return mVertexShaderSource;
}


std::span<const std::byte>
Shader::fragmentShaderSource() const
{
	return mFragmentShaderSource;
}


void
Shader::setVertexShaderSource(std::span<const std::byte> source)
{
	mVertexShaderSource.assign(source.begin(), source.end());
	markOutdated();
}


void
Shader::setFragmentShaderSource(std::span<const std::byte> source)
{
	mFragmentShaderSource.assign(source.begin(), source.end());
	markOutdated();
}
