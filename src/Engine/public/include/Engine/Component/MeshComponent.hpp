#ifndef ENGINE_COMPONENTS_MESHCOMPONENT_HPP
#define ENGINE_COMPONENTS_MESHCOMPONENT_HPP

#include <Engine/Core/Component.hpp>

#include <Engine/Core/Mesh.hpp>

#include <memory>
#include <vector>

namespace Reef
{

class ENGINE_API MeshComponent : public SubclassOf<MeshComponent, Component>
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

#endif //!ENGINE_COMPONENTS_MESHCOMPONENT_HPP