#include "ResourceManager.hpp"

#include "Core/Base.hpp"
#include "Core/Log.hpp"
#include "FileSystem/FileSystem.hpp"

#include "glad/gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace zn
{
    ResourceRegistry<Shader> ResourceManager::s_shadersRegistry;
    ResourceRegistry<Texture> ResourceManager::s_textureRegistry;
    
    Opt<Handle<Shader>> ResourceManager::LoadShader(const String& vertPath, const String& fragPath)
    {
        if (!FileSystem::Exists(vertPath))
        {
            ZN_CORE_WARN("[ResourceManager::LoadShader] Failed to load Shader. File {} does not exist", vertPath);
            return std::nullopt;
        }

        if (!FileSystem::Exists(fragPath))
        {
            ZN_CORE_WARN("[ResourceManager::LoadShader] Failed to load Shader. File {} does not exist", fragPath);
            return std::nullopt;
        }
		
        auto vertexCode = FileSystem::ReadFileAsString(vertPath);
        if (!vertexCode)
        {
            ZN_CORE_WARN("[ResourceManager::LoadShader] Failed to load Shader. Failed to read vertex shader code from {}", vertPath);
            return std::nullopt;
        }

        auto fragmentCode = FileSystem::ReadFileAsString(fragPath);
        if (!fragmentCode)
        {
            ZN_CORE_WARN("[ResourceManager::LoadShader] Failed to load Shader. Failed to read fragment shader code from {}", fragPath);
            return std::nullopt;
        }

        auto handle = s_shadersRegistry.EmplaceResource(vertexCode.value().c_str(), fragmentCode.value().c_str());
        if (!handle.has_value())
        {
            return std::nullopt;
        }
        
        return handle;
    }

    Opt<CRefWrapper<Shader>> ResourceManager::GetShader(Handle<Shader> handle)
    {
        if (Opt<CRefWrapper<Shader>> shader = s_shadersRegistry.GetResourceRef(handle))
        {
            return shader;
        }

        ZN_CORE_WARN("[ResourceManager::GetShader] Failed to retrieve Shader. The provided Shader handle (Id: {}, Gen: {}) is not valid", handle.GetIndex(), handle.GetGeneration());
        
        return std::nullopt;
    }

    bool ResourceManager::ReleaseShader(Handle<Shader> handle)
    {
        return s_shadersRegistry.ReleaseResource(handle);
    }

    Opt<Handle<Texture>> ResourceManager::LoadTexture(const String& path)
    {
        if (!FileSystem::Exists(path))
        {
            ZN_CORE_WARN("[ResourceManager::LoadTexture] Failed to load Texture resource. File {} does not exist", path);
            return std::nullopt;
        }
        
        FileSystem::Path fullPath = FileSystem::GetFullPath(path).value();

        stbi_set_flip_vertically_on_load(1);

        int width, height, channels;

        stbi_uc* data = stbi_load(fullPath.string().c_str(), &width, &height, &channels, 0);
        if (data)
        {
            unsigned int dataFormat;
            unsigned int internalFormat;
            
            if (channels == 4)
            {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else if (channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
            
            auto handle = s_textureRegistry.EmplaceResource(data, width, height, channels, internalFormat, dataFormat);

            stbi_image_free(data);

            return handle;
        }
        
        ZN_CORE_WARN("[ResourceManager::LoadTexture] Failed to load Texture resource. Library (stbi) failed to load texture: {}", path);
        
        return std::nullopt;
    }

    Opt<Handle<Texture>> ResourceManager::LoadTextureTest(const String& path)
    {
        if (!FileSystem::Exists(path))
        {
            ZN_CORE_WARN("[ResourceManager::LoadTexture] Failed to load Texture resource. File {} does not exist", path);
            return std::nullopt;
        }
        
        FileSystem::Path fullPath = FileSystem::GetFullPath(path).value();

        stbi_set_flip_vertically_on_load(1);

        int width, height, channels;

        stbi_uc* data = stbi_load(fullPath.string().c_str(), &width, &height, &channels, 0);
        if (data)
        {
            unsigned int dataFormat;
            unsigned int internalFormat;
            
            if (channels == 4)
            {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else if (channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
            
            //auto handle = s_textureRegistry.CreateResource({data, width, height, channels, internalFormat, dataFormat});

            stbi_image_free(data);

            //return handle;
            return std::nullopt;
        }
        
        ZN_CORE_WARN("[ResourceManager::LoadTexture] Failed to load Texture resource. Library (stbi) failed to load texture: {}", path);
        
        return std::nullopt;
    }

    Opt<CRefWrapper<Texture>> ResourceManager::GetTexture(Handle<Texture> handle)
    {
        if (Opt<CRefWrapper<Texture>> texture = s_textureRegistry.GetResourceRef(handle))
        {
            return texture;
        }

        ZN_CORE_WARN("[ResourceManager::GetTexture] Failed to retrieve Texture. The provided Texture handle (Id: {}, Gen: {}) is not valid", handle.GetIndex(), handle.GetGeneration());
        
        return std::nullopt;
    }

    bool ResourceManager::ReleaseTexture(Handle<Texture> handle)
    {
        return s_textureRegistry.ReleaseResource(handle);
    }

    void ResourceManager::Shutdown()
    {
        
    }
}
