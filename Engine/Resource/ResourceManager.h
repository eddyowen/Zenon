#pragma once

#include "Core/Base.h"

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

#include <optional>
#include <unordered_map>

namespace zn
{
    class ResourceManager
    {
    public:
        using ShaderResource = std::optional<SharedPtr<Shader>>;
        using TextureResource = std::optional<SharedPtr<Texture>>;
        
        using ShaderMap = std::unordered_map<std::string, SharedPtr<Shader>>;
        using TextureMap = std::unordered_map<std::string, SharedPtr<Texture>>;
        
        ~ResourceManager() = default;
        
        ResourceManager(const ResourceManager& other) = delete;
        ResourceManager(ResourceManager&& other) noexcept = delete;
        
        ResourceManager& operator=(const ResourceManager& other) = delete;
        ResourceManager& operator=(ResourceManager&& other) noexcept = delete;
        
        [[nodiscard]]
        static std::optional<SharedPtr<Shader>> LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
        static ShaderResource GetShader(const std::string& name);

        [[nodiscard]]
        static TextureResource LoadTexture(const std::string& resourceName, const std::string& path);
        static TextureResource GetTexture(const std::string& name);

        static void Shutdown();

    private:
        ResourceManager() = default;

        static ShaderMap s_Shaders;
        static TextureMap s_Textures;
    };
}
