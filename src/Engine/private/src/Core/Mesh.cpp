module;

#include <glm/glm.hpp>

#include <array>
#include <cassert>
#include <memory>
#include <numeric>
#include <vector>

module Engine.Mesh;

import Engine.Texture;
import Engine.Material;

using namespace Reef;

std::shared_ptr<Primitive>
Mesh::getPrimitive(uint32_t index)
{
	if (index >= mPrimitives.size())
	{
		return nullptr;
	}

	return mPrimitives[index];
}


std::shared_ptr<const Primitive>
Mesh::getPrimitive(uint32_t index) const
{
	if (index >= mPrimitives.size())
	{
		return nullptr;
	}

	return mPrimitives[index];
}


uint32_t 
Mesh::getNumPrimitives() const
{
	return mPrimitives.size();
}


Primitive&
Mesh::addPrimitive()
{
	auto& part = mPrimitives.emplace_back();
	part = std::make_shared<Primitive>();

	return *part;
}


const std::vector<std::shared_ptr<Primitive>>& 
Mesh::primitives()
{
	return mPrimitives;
}


std::shared_ptr<Material> 
Mesh::material(uint32_t index)
{
	if (index >= mPrimitives.size())
	{
		return nullptr;
	}

	return mPrimitives[index]->material();
}


BoundingBox3D
Mesh::boundingBox() const
{
	return std::accumulate(mPrimitives.begin(), 
						   mPrimitives.end(), BoundingBox3D(),
		[](const BoundingBox3D& acc, std::shared_ptr<Primitive> primitive) { return acc + primitive->getBoundingBox(); });
}


std::shared_ptr<Mesh>
Mesh::Sphere()
{
	static std::shared_ptr<Mesh> sMesh;

	if (!sMesh)
	{
		sMesh = Mesh::Sphere(32);
	}

	return sMesh;
}


std::shared_ptr<Mesh>
Mesh::Sphere(uint32_t numSubdivisions)
{
	//float thetaStepSize = glm::pi<float>() / numSubdivisions;
	//float phiStepSize = Math::TWO_PI / numSubdivisions;
	//
	//std::vector<glm::vec3> positions;
	//std::vector<glm::vec3> normals;
	//std::vector<glm::vec2> uvs;

	//std::vector<uint32_t> indices;

	//for (uint32_t i = 0; i <= numSubdivisions; ++i)
	//{
	//	float theta = -Math::HALF_PI + thetaStepSize * (i);

	//	for (uint32_t j = 0; j <= numSubdivisions; ++j)
	//	{
	//		float phi = j * phiStepSize;

	//		glm::vec2 latLon(phi, theta);
	//		glm::vec3 p = glm::normalize(Math::sphericalCoordsToCartesianCoords(latLon));
	//		positions.push_back(p);
	//		normals.push_back(p);
	//		glm::vec2 scale(Math::TWO_PI, Math::PI);
	//		glm::vec2 offset(0.f, 0.5f);

	//		auto uv = 1.f - latLon / scale + offset;

	//		uvs.push_back(uv);
	//	}
	//}

	//// Build indices
	//for (uint32_t y = 0; y < numSubdivisions; ++y)
	//{
	//	for (uint32_t x = 0; x < numSubdivisions; ++x)
	//	{
	//		auto topLeft		= y * (numSubdivisions + 1) + x;
	//		auto topRight		= y * (numSubdivisions + 1) + x + 1;
	//		auto bottomLeft		= (y + 1) * (numSubdivisions + 1) + x;
	//		auto bottomRight	= (y + 1) * (numSubdivisions + 1) + x + 1;

	//		// Only emit these triangles when not at the north pole
	//		if (y > 0)
	//		{
	//			indices.push_back(topLeft);
	//			indices.push_back(topRight);
	//			indices.push_back(bottomLeft);
	//		}
	//		// Only emit these triangles when not at the south pole
	//		if (y <= numSubdivisions)
	//		{
	//			indices.push_back(topRight);
	//			indices.push_back(bottomRight);
	//			indices.push_back(bottomLeft);
	//		}
	//	}
	//}

	//auto mesh = std::make_shared<Mesh>();
	//auto& primitive = mesh->addPrimitive();
	//primitive.setPositions(positions);
	//primitive.setIndices(indices);
	//primitive.setNormals(normals);
	//primitive.setTexcoords(uvs);
	//primitive.recalculateTangents();
	//primitive.setMaterial(std::make_shared<Material>(*Material::DefaultPBR()));

	//return mesh;
	return nullptr;
}


std::shared_ptr<Mesh>
Mesh::Quad()
{
	static std::shared_ptr<Mesh> sMesh;

	if (!sMesh)
	{
		std::array positions
		{
			glm::vec3(-1.f, -1.f, 0.f),
			glm::vec3( 1.f, -1.f, 0.f),
			glm::vec3(-1.f,  1.f, 0.f),
			glm::vec3( 1.f,  1.f, 0.f),
		};

		std::array uvs
		{
			glm::vec2(0.f, 0.f),
			glm::vec2(1.f, 0.f),
			glm::vec2(0.f, 1.f),
			glm::vec2(1.f, 1.f),
		};

		std::array normals
		{
			glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.f, 0.f, 1.f),
		};

		std::array<uint16_t, 6> indices
		{
			0, 1, 2,
			2, 1, 3
		};
		 
		sMesh = std::make_shared<Mesh>();
		auto& primitive = sMesh->addPrimitive();
		primitive.setPositions(positions);
		primitive.setIndices(indices);
		primitive.setNormals(normals);
		primitive.setTexcoords(uvs);
		primitive.recalculateTangents();
		//primitive.setMaterial(std::make_shared<Material>(*Material::DefaultPBR()));
	}

	return sMesh;
}


std::shared_ptr<Mesh>
Mesh::Cube()
{
	static std::shared_ptr<Mesh> sMesh;

	if (!sMesh)
	{
		std::array positions
		{
			// -z
			glm::vec3{  .5f, -.5f, -.5f },
			glm::vec3{ -.5f, -.5f, -.5f },
			glm::vec3{  .5f,  .5f, -.5f },
			glm::vec3{ -.5f,  .5f, -.5f },

			// +z
			glm::vec3{ -.5f, -.5f,  .5f },
			glm::vec3{  .5f, -.5f,  .5f },
			glm::vec3{ -.5f,  .5f,  .5f },
			glm::vec3{  .5f,  .5f,  .5f },

			// -x
			glm::vec3{ -.5f, -.5f, -.5f },
			glm::vec3{ -.5f, -.5f,  .5f },
			glm::vec3{ -.5f,  .5f, -.5f },
			glm::vec3{ -.5f,  .5f,  .5f },

			// +x
			glm::vec3{ .5f, -.5f,  .5f },
			glm::vec3{ .5f, -.5f, -.5f },
			glm::vec3{ .5f,  .5f,  .5f },
			glm::vec3{ .5f,  .5f, -.5f },

			// -y
			glm::vec3{ -.5f, -.5f, -.5f },
			glm::vec3{  .5f, -.5f, -.5f },
			glm::vec3{ -.5f, -.5f,  .5f },
			glm::vec3{  .5f, -.5f,  .5f },

			// +y
			glm::vec3{ -.5f, .5f,  .5f },
			glm::vec3{  .5f, .5f,  .5f },
			glm::vec3{ -.5f, .5f, -.5f },
			glm::vec3{  .5f, .5f, -.5f },
		};

		std::array uvs
		{
			// -z
			glm::vec2{ 0.f, 0.f },
			glm::vec2{ 1.f, 0.f },
			glm::vec2{ 0.f, 1.f },
			glm::vec2{ 1.f, 1.f },

			// +z
			glm::vec2{ 0.f, 0.f },
			glm::vec2{ 1.f, 0.f },
			glm::vec2{ 0.f, 1.f },
			glm::vec2{ 1.f, 1.f },

			// -x
			glm::vec2{ 0.f, 0.f },
			glm::vec2{ 1.f, 0.f },
			glm::vec2{ 0.f, 1.f },
			glm::vec2{ 1.f, 1.f },

			// +x
			glm::vec2{ 0.f, 0.f },
			glm::vec2{ 1.f, 0.f },
			glm::vec2{ 0.f, 1.f },
			glm::vec2{ 1.f, 1.f },

			// -y
			glm::vec2{ 0.f, 0.f },
			glm::vec2{ 1.f, 0.f },
			glm::vec2{ 0.f, 1.f },
			glm::vec2{ 1.f, 1.f },

			// +y
			glm::vec2{ 0.f, 0.f },
			glm::vec2{ 1.f, 0.f },
			glm::vec2{ 0.f, 1.f },
			glm::vec2{ 1.f, 1.f },

		};

		std::array normals
		{
			// -z
			glm::vec3{ 0.f, 0.f, -1.f },
			glm::vec3{ 0.f, 0.f, -1.f },
			glm::vec3{ 0.f, 0.f, -1.f },
			glm::vec3{ 0.f, 0.f, -1.f },

			// +z
			glm::vec3{ 0.f, 0.f, 1.f },
			glm::vec3{ 0.f, 0.f, 1.f },
			glm::vec3{ 0.f, 0.f, 1.f },
			glm::vec3{ 0.f, 0.f, 1.f },

			// -x
			glm::vec3{ -1.f, 0.f, 0.f },
			glm::vec3{ -1.f, 0.f, 0.f },
			glm::vec3{ -1.f, 0.f, 0.f },
			glm::vec3{ -1.f, 0.f, 0.f },

			// +x
			glm::vec3{ 1.f, 0.f, 0.f },
			glm::vec3{ 1.f, 0.f, 0.f },
			glm::vec3{ 1.f, 0.f, 0.f },
			glm::vec3{ 1.f, 0.f, 0.f },

			// -y
			glm::vec3{ 0.f, -1.f, 0.f },
			glm::vec3{ 0.f, -1.f, 0.f },
			glm::vec3{ 0.f, -1.f, 0.f },
			glm::vec3{ 0.f, -1.f, 0.f },

			// +y
			glm::vec3{ 0.f, 1.f, 0.f },
			glm::vec3{ 0.f, 1.f, 0.f },
			glm::vec3{ 0.f, 1.f, 0.f },
			glm::vec3{ 0.f, 1.f, 0.f },

		};

		std::array<uint16_t, 36> indices
		{
			// -z
			0, 1, 2,
			2, 1, 3,

			// +z
			4, 5, 6,
			6, 5, 7,

			// -x
			8, 9, 10,
			10, 9, 11,

			// +x
			12, 13, 14,
			14, 13, 15,

			// -y
			16, 17, 18,
			18, 17, 19,

			// +y
			20, 21, 22,
			22, 21, 23,
		};

		sMesh = std::make_shared<Mesh>();
		auto& primitive = sMesh->addPrimitive();

		primitive.setPositions(positions);
		primitive.setIndices(indices);
		primitive.setNormals(normals);
		
		primitive.setTexcoords(uvs);
		primitive.recalculateTangents();
		//primitive.setMaterial(std::make_shared<Material>(*Material::DefaultPBR()));
	}

	return sMesh;
}


std::shared_ptr<Mesh>
Mesh::Plane()
{
	static std::shared_ptr<Mesh> sMesh;

	if (!sMesh)
	{
		constexpr static auto Size = 10;
		constexpr static auto NumVertices = Size * Size;
		std::array<glm::vec3, NumVertices> positions;
		std::array<glm::vec3, NumVertices> normals;
		std::array<glm::vec2, NumVertices> uvs;
		constexpr static auto NumFaces = (Size - 1) * (Size - 1) * 2;
		std::array<uint16_t, NumFaces * 3> indices;

		// Build the grid
		for (float z = 0; z < Size; ++z)
		{
			for (float x = 0; x < Size; ++x)
			{
				positions[z * Size + x] = glm::vec3(x - Size * 0.5f, 0.f, z - Size * 0.5f);
				normals[z * Size + x]	= glm::vec3(0.f, 1.f, 0.f);
				uvs[z * Size + x]		= glm::vec2(x / (Size - 1.0f), z / (Size - 1.0f));
			}
		}

		uint16_t i = 0;
		// Build the triangles
		for (uint16_t z = 0; z < Size - 1; ++z)
		{
			for (uint16_t x = 0; x < Size - 1; ++x)
			{
				uint16_t bottomLeft = z * Size + x;
				uint16_t bottomRight = z * Size + x + 1;
				uint16_t topLeft = (z + 1) * Size + x;
				uint16_t topRight = (z + 1) * Size + x + 1;

				indices[i++] = bottomRight;
				indices[i++] = bottomLeft;
				indices[i++] = topLeft;
				indices[i++] = bottomRight;
				indices[i++] = topLeft;
				indices[i++] = topRight;
			}
		}

		sMesh = std::make_shared<Mesh>();
		auto& primitive = sMesh->addPrimitive();
		
		primitive.setPositions(positions);
		primitive.setIndices(indices);
		primitive.setNormals(normals);
		primitive.setTexcoords(uvs);
		primitive.recalculateTangents();
		//primitive.setMaterial(std::make_shared<Material>(*Material::DefaultPBR()));
	}

	return sMesh;
}