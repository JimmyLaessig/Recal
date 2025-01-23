#include <Engine/Core/Transform.hpp>
#include <Engine/Core/SceneObject.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>

using namespace Reef;


Transform::Transform(SceneObject& gameObject)
	: mGameObject(gameObject)
{
}


const glm::mat4&
Transform::modelMatrix() const
{
	if (!mModelMatrix)
	{
		auto T = glm::translate(mPosition);
		auto R = glm::toMat4(mRotation);
		auto S = glm::scale(mScale);

		mModelMatrix = T * R * S;
	}

	return *mModelMatrix;
}


const glm::mat4&
Transform::inverseModelMatrix() const
{
	if (!mInverseModelMatrix)
	{
		mInverseModelMatrix = glm::inverse(modelMatrix());
	}

	return *mInverseModelMatrix;
}


const glm::vec3&
Transform::localPosition() const
{
	return mPosition;
}


const glm::vec3&
Transform::localScale() const
{
	return mScale;
}


const glm::quat&
Transform::localRotation() const
{
	return mRotation;
}


glm::vec3
Transform::worldPosition() const
{
	if (auto parent = mGameObject.parent())
	{
		auto wp = parent->transform().localToWorldMatrix()* glm::vec4(mPosition, 1.f);

		return glm::vec3(wp / wp[3]);
	}

	return mPosition;
}


glm::vec3
Transform::worldScale() const
{
	if (auto parent = mGameObject.parent())
	{
		return parent->transform().worldScale() * mScale;
	}

	return mScale;
}


glm::quat
Transform::worldRotation() const
{
	if (auto parent = mGameObject.parent())
	{
		return parent->transform().worldRotation() * mRotation;
	}

	return mRotation;
}


void
Transform::setLocalPosition(const glm::vec3& position)
{
	mPosition = position;
	mModelMatrix.reset();
	mInverseModelMatrix.reset();
}


void
Transform::setLocalScale(const glm::vec3& scale)
{
	mScale = scale;
	mModelMatrix.reset();
	mInverseModelMatrix.reset();
}


void
Transform::setLocalRotation(const glm::quat& rotation)
{
	mRotation = rotation;
	mModelMatrix.reset();
	mInverseModelMatrix.reset();
}


void 
Transform::setLocalPose(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
{
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mModelMatrix.reset();
	mInverseModelMatrix.reset();
}


void
Transform::setWorldPosition(const glm::vec3& position)
{
	if (auto parent = mGameObject.parent())
	{
		auto pos = parent->transform().worldToLocalMatrix() * glm::vec4(position, 1.0f);

		mPosition = glm::vec3(pos / pos.w);
	}
	else
	{
		mPosition = position;
	}

	mModelMatrix.reset();
	mInverseModelMatrix.reset();
}


void
Transform::setWorldScale(const glm::vec3& scale)
{
	if (auto parent = mGameObject.parent())
	{
		mScale = scale / parent->transform().worldScale();
	}
	else
	{
		mScale = scale;
	}
	mModelMatrix.reset();
	mInverseModelMatrix.reset();
}


void
Transform::setWorldRotation(const glm::quat& rotation)
{
	if (auto parent = mGameObject.parent())
	{
		mRotation = parent->transform().worldRotation() * rotation;
	}
	else
	{
		mRotation = rotation;
	}
	mModelMatrix.reset();
	mInverseModelMatrix.reset();
}


void
Transform::setWorldPose(const glm::vec3& worldPosition, const glm::quat& worldRotation, const glm::vec3& worldScale)
{
	setWorldPosition(worldPosition);
	setWorldRotation(worldRotation);
	setWorldScale(worldScale);
}


glm::mat4
Transform::localToWorldMatrix() const
{
	if (auto parent = mGameObject.parent())
	{
		return parent->transform().localToWorldMatrix() * modelMatrix();
	}

	return modelMatrix();
}


glm::mat4
Transform::worldToLocalMatrix() const
{
	// TODO: Make this more performant
	return glm::inverse(localToWorldMatrix());
}


glm::vec3 
Transform::forward() const
{
	glm::vec3 v(0.f, 0.f, -1.f);

	return glm::normalize(mRotation * v);
}


glm::vec3 
Transform::right() const
{
	glm::vec3 v(-1.f, 0.f, 0.f);
	return glm::normalize(mRotation * v);
}


glm::vec3 
Transform::up() const
{
	glm::vec3 v(0.f, 1.f, 0.f);
	return glm::normalize(mRotation * v);
}


void
Transform::lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
	auto modelMatrix = glm::inverse(glm::lookAt(eye, center, up));
	
	// TODO: Fix
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(modelMatrix, scale, rotation, translation, skew, perspective);

	setWorldPosition(translation);
	setWorldRotation(rotation);
}
