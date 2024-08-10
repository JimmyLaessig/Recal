module;

#include <Engine/System.h>

#include <glm/glm.hpp>

#include <memory>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <cstdint>
#include <string>

export module Engine.Material;

import Engine.Object;
import Engine.SubclassOf;
import Engine.Texture;

export namespace Reef
{

class ENGINE_API Material : public Reef::SubclassOf<Material, Object>
{

public:

	Material();

	enum class Mode
	{
		LIT,
		UNLIT,
	};

	using ParameterValue = std::variant<float, glm::vec2, glm::vec3, glm::vec4,
										int, glm::ivec2, glm::ivec3, glm::ivec4,
										glm::mat2, glm::mat3, glm::mat4,
										std::shared_ptr<Texture>>;

	bool castShadow() const { return mCastsShadow; };

	void setCastsShadow(bool value) { mCastsShadow = value; }

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

	void setMode(Mode mode);

	Mode mode() const;

	//void setShader(std::shared_ptr<Reef::Shader> shader);

	//std::shared_ptr<Engine::Shader> shader();

	std::unordered_map<uint32_t, ParameterValue> parameters() const;
	
	static std::shared_ptr<Material> DefaultPBR();

	bool isOutdated() const { return mIsOutdated; }

private:

	bool setParameterImpl(std::string_view name, ParameterValue value);

	mutable bool mIsOutdated{ true };

	bool mCastsShadow{ true };

	Mode mMode{ Mode::LIT };

	std::unordered_map<std::string, uint32_t> mNameToLocationMapping;

	std::unordered_map<uint32_t, ParameterValue> mParameters;

	//std::shared_ptr<Reef::Shader> mShader;
};

} // namespace Reef