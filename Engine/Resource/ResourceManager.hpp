#pragma once

#include "Core/Base.hpp"
#include "Core/Log.hpp"

#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

namespace zn
{
    class ResourceManager
    {
    public:
        using ShaderResource = Opt<SharedPtr<Shader>>;
        using TextureResource = Opt<SharedPtr<Texture>>;
        
        using ShaderMap = UMap<String, SharedPtr<Shader>>;
        using TextureMap = UMap<String, SharedPtr<Texture>>;
        
        ~ResourceManager() = default;
        
        ResourceManager(const ResourceManager& other) = delete;
        ResourceManager(ResourceManager&& other) noexcept = delete;
        
        ResourceManager& operator=(const ResourceManager& other) = delete;
        ResourceManager& operator=(ResourceManager&& other) noexcept = delete;
        
        [[nodiscard]]
        static Opt<SharedPtr<Shader>> LoadShader(const String& name, const String& vertPath, const String& fragPath);
        static ShaderResource GetShader(const String& name);

        [[nodiscard]]
        static TextureResource LoadTexture(const String& resourceName, const String& path);
        static TextureResource GetTexture(const String& name);

        static void Shutdown();

    private:
        ResourceManager() = default;

        static ShaderMap s_shaders;
        static TextureMap s_textures;
    };
}
