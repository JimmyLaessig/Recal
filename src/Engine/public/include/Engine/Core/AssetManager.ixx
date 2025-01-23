module;

#include <memory>
#include <unordered_map>
#include <string>
#include <string_view>
#include <Engine/System.hpp>

export module Engine.AssetManager;


import Engine.Shader;


export namespace Reef
{

class ENGINE_API AssetManager
{
public:

    std::shared_ptr<Reef::Shader> findShader(std::string_view name);

    bool addShader(std::string_view, std::shared_ptr<Reef::Shader> shader);

private:

    std::unordered_map<std::string, std::shared_ptr<Reef::Shader>> mShaders;
};

} // export namespace Reef
