module;

#include <Coral/System.h>

#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <span>

export module Coral.ShaderModule;

import Coral.Types;


export namespace Coral
{
class Context;

enum class ShaderStage
{
	VERTEX,
	FRAGMENT
};


struct ShaderModuleConfig
{
	std::string_view name;

	ShaderStage stage;

	std::span<const std::byte> source;
};


enum class ValueType
{
	BOOL,
	INT,
	FLOAT,
	VEC2F,
	VEC3F,
	VEC4F,
	MAT33F,
	MAT44F
};


struct ValueDescription
{
	ValueType type;
	std::string name;
	size_t count{ 1 };
};


struct UniformBlockDescription
{
	std::vector<ValueDescription> members;
};


struct SamplerDescription{};

struct AttributeDescription
{
	uint32_t binding{ 0 };
	uint32_t location{ 0 };
	Coral::AttributeFormat format;
	std::string name;
};

using DescriptorType = std::variant<SamplerDescription, UniformBlockDescription>;

struct BindingDescription
{
	uint32_t set{ 0 };
	uint32_t binding{ 0 };
	std::string name;
	size_t byteSize{ 0 };
	DescriptorType type{ SamplerDescription{} };
};


class CORAL_API ShaderModule
{
public:

	virtual ~ShaderModule() = default;

	virtual ShaderStage shaderStage() const = 0;

	virtual std::string_view name() const = 0;

	virtual std::string_view entryPoint() const = 0;

	virtual std::span<const AttributeDescription> inputDescriptions() const = 0;

	virtual std::span<const AttributeDescription> outputDescriptions() const = 0;

	virtual std::span<const BindingDescription> bindingDescriptions() const = 0;
};


CORAL_API ShaderModule* createShaderModule(Context* context, const ShaderModuleConfig& config);

CORAL_API void destroy(ShaderModule* shaderModule);

} // namespace Coral
