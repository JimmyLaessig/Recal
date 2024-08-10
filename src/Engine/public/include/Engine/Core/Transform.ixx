module;

#include <Engine/System.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <optional>

export module Engine.Transform;

import Common.NonCopyable;

export namespace Reef
{

class GameObject;

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

	friend class GameObject;

	Transform(GameObject& gameObject);

	GameObject& mGameObject;

	glm::vec3 mPosition;

	glm::vec3 mScale{ 1.f, 1.f, 1.f };

	glm::quat mRotation;

	mutable std::optional<glm::mat4> mModelMatrix;

	mutable std::optional<glm::mat4> mInverseModelMatrix;
};

} // export namespace Reef