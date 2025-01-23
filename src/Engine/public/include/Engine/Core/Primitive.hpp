#ifndef ENGINE_CORE_PRIMITIVE_HPP
#define	ENGINE_CORE_PRIMITIVE_HPP

#include <Engine/System.hpp>

#include <Engine/Core/Resource.hpp>

#include <Engine/Core/BoundingBox3D.hpp>
#include <Engine/Core/SubclassOf.hpp>
#include <Engine/Core/Material.hpp>

#include <glm/glm.hpp>

#include <cstdint>
#include <memory>
#include <span>
#include <unordered_map>
#include <vector>


namespace Reef
{

enum class IndexFormat
{
	UNSIGNED_SHORT,
	UNSIGNED_INT
};


class ENGINE_API Primitive : public SubclassOf<Primitive, Resource>
{
public:
	
	Primitive();

	// Set the attributes of the primitive
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	bool set(std::span<const uint16_t> indices,
			 std::span<const glm::vec3> positions, 
			 std::span<const glm::vec3> normals, 
			 std::span<const glm::vec3> tangents,
			 std::span<const glm::vec2> texCoords);

	// Set the attributes of the primitive
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	bool set(std::span<const uint32_t> indices,
			 std::span<const glm::vec3> positions,
			 std::span<const glm::vec3> normals,
			 std::span<const glm::vec3> tangents,
			 std::span<const glm::vec2> texCoords);

	// Get the format of the indices
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	IndexFormat indexFormat() const;

	// Get the primitive's indices
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	std::span<const std::byte> indices() const;

	// Get the primitive's per-vertex position attribute
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	std::span<const glm::vec3> positions() const;

	// Get the primitive's per-vertex normal attribute
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	std::span<const glm::vec3> normals() const;

	// Get the primitive's per-vertex tangent attribute
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	std::span<const glm::vec3> tangents() const;

	// Get the primitive's per-vertex texture coordinate attribute
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	std::span<const glm::vec2> texcoords() const;

	// Get the primitive's indices
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	std::shared_ptr<Material> material();

	// Get the primitive's material
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	std::shared_ptr<const Material> material() const;

	// Set the primitive's material
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	void setMaterial(std::shared_ptr<Material> material);

	// Get the bounding box of the primitive
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	const BoundingBox3D& getBoundingBox() const;

private:

	bool set(std::span<const std::byte> indices,
			 IndexFormat indexFormat,
			 std::span<const glm::vec3> positions,
			 std::span<const glm::vec3> normals,
			 std::span<const glm::vec3> tangents,
			 std::span<const glm::vec2> texCoords);

	void recalculateTangents();

	void recalculateNormals();

	void recalculateBoundingBox();

	IndexFormat mIndexFormat{ IndexFormat::UNSIGNED_SHORT };

	std::vector<std::byte> mIndices;

	std::vector<glm::vec3> mPositions;

	std::vector<glm::vec3> mNormals;

	std::vector<glm::vec3> mTangents;

	std::vector<glm::vec2> mTexCoords;

	BoundingBox3D mBoundingBox{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f) };

	std::shared_ptr<Material> mMaterial;
};

} // namespace Reef

#endif // !ENGINE_CORE_PRIMITIVE_HPP