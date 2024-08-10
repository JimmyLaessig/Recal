module;

#include <Coral/System.h>

#include <variant>
#include <vector>

export module Coral.DescriptorSet;

export namespace Coral
{
class Context;
class Buffer;
class Sampler;

using Descriptor = std::variant<Buffer*, Sampler*>;

struct DescriptorBinding
{
	uint32_t binding{ 0 };

	Descriptor descriptor;
};


/// Configuration to create a Buffer

struct DescriptorSetConfig
{
	std::vector<DescriptorBinding> bindings;
};

struct DescriptorSetLayout;

class CORAL_API DescriptorSet
{
public:

	virtual ~DescriptorSet() = default;

};


/// Create a new DescriptorSet
CORAL_API DescriptorSet* createDescriptorSet(Context* context, const DescriptorSetConfig& config);

/// Destroy the DescriptorSet
CORAL_API void destroy(DescriptorSet* uniformDescriptorSet);

} // namespace Coral