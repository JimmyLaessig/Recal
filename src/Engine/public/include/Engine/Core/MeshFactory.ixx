module;

#include <memory>

#include <tiny_gltf.h>

export module Engine.MeshFactory;

import Engine.Mesh;

export namespace Reef
{

class MeshFactory
{
public:

	static std::shared_ptr<Mesh> createMesh(const tinygltf::Mesh& mesh, const tinygltf::Model& model);
};

} // namespace Reef