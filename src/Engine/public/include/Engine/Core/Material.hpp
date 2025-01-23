#ifndef ENGINE_CORE_MATERIAL_HPP
#define ENGINE_CORE_MATERIAL_HPP

#include <Engine/System.hpp>

#include <Engine/Core/SubclassOf.hpp>
#include <Engine/Core/Texture.hpp>

#include <Engine/Rendering/Shader.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <cstdint>
#include <string>

#include <Coral/UniformBlockBuilder.hpp>

namespace Reef
{

class ENGINE_API Material : public Reef::SubclassOf<Material, Resource>
{

public:

	using ParameterValue = std::variant<float, glm::vec2, glm::vec3, glm::vec4,
										int, glm::ivec2, glm::ivec3, glm::ivec4,
										glm::mat2, glm::mat3, glm::mat4,
										std::shared_ptr<Texture>>;

	//bool castShadow() const { return mCastsShadow; };

	//void setCastsShadow(bool value) { mCastsShadow = value; }

	bool setParameter(std::string_view name, float value);

	bool setParameter(std::string_view name, const glm::vec2& value);

	bool setParameter(std::string_view name, const glm::vec3& value);

	bool setParameter(std::string_view name, const glm::vec4& value);

	bool setParameter(std::string_view name, int value);

	bool setParameter(std::string_view name, const glm::ivec2& value);

	bool setParameter(std::string_view name, const glm::ivec3& value);

	bool setParameter(std::string_view name, const glm::ivec4& value);

	bool setParameter(std::string_view name, const glm::mat2& value);

	bool setParameter(std::string_view name, const glm::mat3& value);

	bool setParameter(std::string_view name, const glm::mat4& value);

	bool setParameter(std::string_view name, std::shared_ptr<Reef::Texture> texture);

	std::optional<ParameterValue> getParameter(std::string_view name) const;

	//std::unordered_map<uint32_t, ParameterValue> parameters() const;

	void setShader(std::shared_ptr<Reef::Rendering::Shader> shader);

	std::shared_ptr<Reef::Rendering::Shader> shader() const;

private:

	bool setParameterImpl(std::string_view name, ParameterValue value);

	//bool mCastsShadow{ true };

	//std::unordered_map<std::string, uint32_t> mNameToLocationMapping;

	std::shared_ptr<Reef::Rendering::Shader> mShader;

	std::unordered_map<std::string, ParameterValue> mParameters;
};

} // namespace Reef

#endif // !#define ENGINE_CORE_MATERIAL_HPP