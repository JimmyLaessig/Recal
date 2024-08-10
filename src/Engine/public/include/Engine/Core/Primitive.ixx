module;

#include <Engine/System.h>

#include <glm/glm.hpp>

#include <cstdint>
#include <memory>
#include <span>
#include <unordered_map>
#include <vector>

export module Engine.Primitive;


import Engine.Object;
import Engine.SubclassOf;

import Engine.BoundingBox3D;
import Engine.Material;

export namespace Reef
{

enum class IndexFormat
{
	UNSIGNED_SHORT,
	UNSIGNED_INT
};


class ENGINE_API Primitive : public SubclassOf<Primitive, Object>
{
public:
	
	Primitive();

	void setIndices(const std::span<uint32_t> indices);

	void setIndices(const std::span<uint16_t> indices);

	void setPositions(const std::span<glm::vec3> positions);

	void setNormals(const std::span<glm::vec3> normals);

	void setTexcoords(const std::span<glm::vec2> texCoords);

	void setTangents(const std::span<glm::vec3> tangents);

	IndexFormat indexFormat() const;

	const std::vector<std::byte>& indices() const;

	const std::vector<glm::vec3>& positions() const;

	const std::vector<glm::vec3>& normals() const;

	const std::vector<glm::vec3>& tangents() const;

	const std::vector<glm::vec2>& texcoords() const;

	std::shared_ptr<Material> material();

	std::shared_ptr<const Material> material() const;

	void setMaterial(std::shared_ptr<Material> material);

	const BoundingBox3D& getBoundingBox() const;

	void recalculateTangents();

	size_t version() const;

private:

	void recalculateBoundingBox();

	IndexFormat mIndexFormat{ IndexFormat::UNSIGNED_SHORT };

	std::vector<std::byte> mIndices;

	std::vector<glm::vec3> mPositions;

	std::vector<glm::vec3> mNormals;

	std::vector<glm::vec3> mTangents;

	std::vector<glm::vec2> mTexCoords;

	BoundingBox3D mBoundingBox{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f) };

	std::shared_ptr<Material> mMaterial;

	mutable size_t mVersion{ 0 };

	mutable bool mIsOutdated{ true };
};

} // namespace Reef