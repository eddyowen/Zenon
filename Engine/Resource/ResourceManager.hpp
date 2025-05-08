#pragma once

#include "Core/Base.hpp"
#include "Core/Log.hpp"
#include "Resource/ResourceRegistry.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

namespace zn
{
    class ResourceManager
    {
    public:
        ~ResourceManager() = default;
        
        ResourceManager(const ResourceManager& other) = delete;
        ResourceManager(ResourceManager&& other) noexcept = delete;
        
        ResourceManager& operator=(const ResourceManager& other) = delete;
        ResourceManager& operator=(ResourceManager&& other) noexcept = delete;
        
        [[nodiscard]] static Opt<Handle<Shader>> LoadShader(const String& vertPath, const String& fragPath);
        [[nodiscard]] static Opt<CRefWrapper<Shader>> GetShader(Handle<Shader> handle);
        [[nodiscard]] static bool ReleaseShader(Handle<Shader> handle);

        [[nodiscard]] static Opt<Handle<Texture>> LoadTexture(const String& path);
        [[nodiscard]] static Opt<Handle<Texture>> LoadTextureTest(const String& path);
        [[nodiscard]] static Opt<CRefWrapper<Texture>> GetTexture(Handle<Texture> handle);
        [[nodiscard]] static bool ReleaseTexture(Handle<Texture> handle);

        static void Shutdown();

    private:
        ResourceManager() = default;

        static ResourceRegistry<Shader> s_shadersRegistry;
        static ResourceRegistry<Texture> s_textureRegistry;
    };
}
