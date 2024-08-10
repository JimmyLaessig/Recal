module;

#include <memory>
#include <vector>

export module Engine.Component.MeshComponent;

import Engine.Component;
import Engine.Mesh;
import Engine.Material;
import Engine.SubclassOf;


export namespace Reef
{

class MeshComponent : public SubclassOf<MeshComponent, Component>
{
public:

	std::shared_ptr<Mesh> mesh();

	std::shared_ptr<const Mesh> mesh() const;

	void setMesh(std::shared_ptr<Mesh> mesh);

	bool setMaterial(std::shared_ptr<Material> material, uint32_t primitiveIndex);

	std::shared_ptr<Material> material(uint32_t index);

private:

	std::shared_ptr<Mesh> mMesh{ nullptr };

	std::vector<std::shared_ptr<Material>> mMaterialOverride;
};

} // namespace Reef