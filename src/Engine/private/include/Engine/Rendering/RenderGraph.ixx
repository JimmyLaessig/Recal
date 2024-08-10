module;

#include <memory>
#include <vector>

export module Engine.Rendering.SceneRenderer;

import Engine.Object;
import Engine.SubclassOf;

namespace Reef
{

namespace Rendering
{

export class RenderGraph
{
public:

	class Node : public Reef::SubclassOf<Node, Reef::Object>
	{
	public:

	};

private:

	std::vector<std::unique_ptr<Node>> mNodes;

};

} // namespace Rendering

} // namespace Reef