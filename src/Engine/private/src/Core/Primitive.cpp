module;

#include <glm/glm.hpp>

#include <cassert>
#include <cstdint>
#include <vector>
#include <span>
#include <array>
#include <memory>

module Engine.Primitive;

import Engine.Texture;
import Engine.Material;


using namespace Reef;

namespace
{

template<typename T>
void copy(const std::span<T> src, std::vector<std::byte>& target)
{
	target.resize(src.size_bytes());
	std::memcpy(target.data(), src.data(), target.size());
}


template<typename IndexType>
std::vector<glm::vec3>
computeTangents(const std::span<IndexType> indices, const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texcoords)
{
	std::vector<glm::vec3> tangents(positions.size());

	for (uint32_t i = 0; i < indices.size(); i += 3)
	{
		const auto& p0 = positions[indices[i    ]];
		const auto& p1 = positions[indices[i + 1]];
		const auto& p2 = positions[indices[i + 2]];

		const auto& uv0 = texcoords[indices[i    ]];
		const auto& uv1 = texcoords[indices[i + 1]];
		const auto& uv2 = texcoords[indices[i + 2]];

		auto& t0 = tangents[indices[i    ]];
		auto& t1 = tangents[indices[i + 1]];
		auto& t2 = tangents[indices[i + 2]];

		auto e01 = p1 - p0;
		auto e02 = p2 - p0;

		auto deltaUV01 = uv1 - uv0;
		auto deltaUV02 = uv2 - uv0;

		float f = 1.0f / (deltaUV01.x * deltaUV02.y - deltaUV02.x * deltaUV01.y);

		t0.x = f * (deltaUV02.y * e01.x - deltaUV01.y * e02.x);
		t0.y = f * (deltaUV02.y * e01.y - deltaUV01.y * e02.y);
		t0.z = f * (deltaUV02.y * e01.z - deltaUV01.y * e02.z);

		t0 = glm::normalize(t0);
		t1 = t0;
		t2 = t0;
	}

	return tangents;
}


std::vector<glm::vec3>
computeTangents(const std::vector<std::byte>& indices,
				IndexFormat indexType,
				const std::vector<glm::vec3>& positions,
				const std::vector<glm::vec2>& texcoords)
{
	switch (indexType)
	{
		case IndexFormat::UNSIGNED_INT:
			return computeTangents(std::span<uint32_t>((uint32_t*)indices.data(), indices.size() / 4), positions, texcoords);
		case IndexFormat::UNSIGNED_SHORT:
			return computeTangents(std::span<uint16_t>((uint16_t*)indices.data(), indices.size() / 2), positions, texcoords);
	}
	assert(false);
	return {};
}

} // namespace


Primitive::Primitive()
{

}


void
Primitive::setIndices(const std::span<uint32_t> indices)
{
	copy(indices, mIndices);
	mIndexFormat = IndexFormat::UNSIGNED_INT;
	mIsOutdated = true;
}


void
Primitive::setIndices(const std::span<uint16_t> indices)
{
	copy(indices, mIndices);
	mIndexFormat = IndexFormat::UNSIGNED_SHORT;
	mIsOutdated = true;
}


void
Primitive::setPositions(const std::span<glm::vec3> positions)
{
	mPositions.assign(positions.begin(), positions.end());
	recalculateBoundingBox();
	mIsOutdated = true;
}


void
Primitive::setNormals(const std::span<glm::vec3> normals)
{
	mNormals.assign(normals.begin(), normals.end());
	mIsOutdated = true;
}


void
Primitive::setTexcoords(const std::span<glm::vec2> texCoords)
{
	mTexCoords.assign(texCoords.begin(), texCoords.end());
	mIsOutdated = true;
}


void
Primitive::setTangents(const std::span<glm::vec3> tangents)
{
	mTangents.assign(tangents.begin(), tangents.end());
	mIsOutdated = true;
	mVersion++;
}


const std::vector<std::byte>&
Primitive::indices() const
{
	return mIndices;
}


IndexFormat
Primitive::indexFormat() const
{
	return mIndexFormat;
}


const std::vector<glm::vec3>&
Primitive::positions() const
{
	return mPositions;
}


const std::vector<glm::vec3>&
Primitive::normals() const
{
	return mNormals;
}


const std::vector<glm::vec3>&
Primitive::tangents() const
{
	return mTangents;
}


const std::vector<glm::vec2>&
Primitive::texcoords() const
{
	return mTexCoords;
}


std::shared_ptr<Material>
Primitive::material()
{
	return mMaterial;
}


std::shared_ptr<const Material>
Primitive::material() const
{
	return mMaterial;
}


void
Primitive::setMaterial(std::shared_ptr<Material> material)
{
	mMaterial = material;
}


const BoundingBox3D&
Primitive::getBoundingBox() const
{
	return mBoundingBox;
}


void
Primitive::recalculateBoundingBox()
{
	mBoundingBox = BoundingBox3D();

	for (auto& p : mPositions)
	{
		mBoundingBox += p;
	}
}


void
Primitive::recalculateTangents()
{
	switch (mIndexFormat)
	{
		case IndexFormat::UNSIGNED_INT:
			mTangents = computeTangents(std::span<uint32_t>((uint32_t*)mIndices.data(), mIndices.size() / 4), mPositions, mTexCoords);
			break;
		case IndexFormat::UNSIGNED_SHORT:
			mTangents = computeTangents(std::span<uint16_t>((uint16_t*)mIndices.data(), mIndices.size() / 2), mPositions, mTexCoords);
			break;
	}

	mIsOutdated = true;
}


size_t
Primitive::version() const
{
	if (mIsOutdated)
	{
		mVersion++;
		mIsOutdated = false;
	}

	return mVersion;
}