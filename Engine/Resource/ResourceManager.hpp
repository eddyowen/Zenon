#pragma once

#include "Core/Base.hpp"

#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

namespace zn
{

    class Handle
    {
    public:
        static constexpr u32 GEN_SHIFT = 32;
        static constexpr u32 INDEX_MASK = 0xFFFFFFFF;
        static constexpr u32 INDEX_MAX_VALUE = 0xFFFFFFFF;
        
        Handle(u32 index, u32 generation)
            : m_value(static_cast<u64>(generation) << GEN_SHIFT | index){}

        u32 GetIndex() const { return static_cast<u32>(m_value & INDEX_MASK); }
        u32 GetGeneration() const { return static_cast<u32>(m_value >> GEN_SHIFT); }
        
    private:
        u64 m_value = 0;
    };
    
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
