#ifndef ENGINE_CORE_TRANSFORM_HPP
#define ENGINE_CORE_TRANSFORM_HPP

#include <Engine/System.hpp>

#include <Common/NonCopyable.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <optional>

namespace Reef
{

class SceneObject;

struct ENGINE_API Transform : public Common::NonCopyable
{
public:

	const glm::mat4& modelMatrix() const;

	const glm::mat4& inverseModelMatrix() const;

	const glm::vec3& localPosition() const;

	const glm::vec3& localScale() const;

	const glm::quat& localRotation() const;

	glm::vec3 worldPosition() const;

	glm::vec3 worldScale() const;

	glm::quat worldRotation() const;

	void setLocalPosition(const glm::vec3& position);

	void setLocalScale(const glm::vec3& scale);

	void setLocalRotation(const glm::quat& rotation);

	void setLocalPose(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

	void setWorldPosition(const glm::vec3& position);

	void setWorldScale(const glm::vec3& scale);

	void setWorldRotation(const glm::quat& rotation);

	void setWorldPose(const glm::vec3& worldPosition, const glm::quat& worldRotation, const glm::vec3& worldScale);

	glm::mat4 localToWorldMatrix() const;

	glm::mat4 worldToLocalMatrix() const;

	glm::vec3 forward() const;

	glm::vec3 right() const;

	glm::vec3 up() const;

	void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

private:

	friend class SceneObject;

	Transform(SceneObject& gameObject);

	SceneObject& mGameObject;

	glm::vec3 mPosition{ 0.f, 0.f, 0.f };

	glm::vec3 mScale{ 1.f, 1.f, 1.f };

	glm::quat mRotation{ 1.f, 0.f, 0.f, 0.f };

	mutable std::optional<glm::mat4> mModelMatrix;

	mutable std::optional<glm::mat4> mInverseModelMatrix;
};

} // namespace Reef

#endif // !ENGINE_CORE_TRANSFORM_HPP