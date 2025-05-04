#include "ResourceManager.hpp"

#include "Core/Base.hpp"
#include "Core/Log.hpp"
#include "FileSystem/FileSystem.hpp"

#include "glad/gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace zn
{
    ResourceManager::ShaderMap ResourceManager::s_shaders;
    ResourceManager::TextureMap ResourceManager::s_textures;
    
    ResourceManager::ShaderResource ResourceManager::LoadShader(
        const String& resourceName,
        const String& vertPath,
        const String& fragPath)
    {
        if (!FileSystem::Exists(vertPath))
        {
            ZN_CORE_ERROR("[ResourceManager::LoadShader] Failed to load shader. File {} does not exist", vertPath);
            return std::nullopt;
        }

        if (!FileSystem::Exists(fragPath))
        {
            ZN_CORE_ERROR("[ResourceManager::LoadShader] Failed to load shader. File {} does not exist", fragPath);
            return std::nullopt;
        }
		
        auto vertexCode = FileSystem::ReadFileAsString(vertPath);
        if (!vertexCode)
        {
            ZN_CORE_ERROR("[ResourceManager::LoadShader] Failed to load shader. Failed to read vertex shader code from {}", vertPath);
            return std::nullopt;
        }

        auto fragmentCode = FileSystem::ReadFileAsString(fragPath);
        if (!fragmentCode)
        {
            ZN_CORE_ERROR("[ResourceManager::LoadShader] Failed to load shader. Failed to read fragment shader code from {}", fragPath);
            return std::nullopt;
        }
        
        if (auto it = s_shaders.find(resourceName); it != s_shaders.end())
        {
            ZN_CORE_ERROR("[ResourceManager::LoadShader] Failed to load shader. Shader ({}) resource already exists in the registry", resourceName);
            return std::nullopt;
        }

        SharedPtr<Shader> shader = CreateShared<Shader>(vertexCode.value().c_str(), fragmentCode.value().c_str());
        s_shaders[resourceName] = shader;
        
        return shader;
    }

    ResourceManager::ShaderResource ResourceManager::GetShader(const String& name)
    {
        auto it = s_shaders.find(name);
        if (it == s_shaders.end())
        {
            ZN_CORE_ERROR("[ResourceManager::GetShader] Failed to retrieve shader. Shader ({}) resource doesn't exist in the registry", name);
            return std::nullopt;
        }
        
        return it->second;
    }

    ResourceManager::TextureResource ResourceManager::LoadTexture(const String& resourceName, const String& path)
    {
        if (!FileSystem::Exists(path))
        {
            ZN_CORE_ERROR("[ResourceManager::LoadTexture] Failed to load texture resource. File {} does not exist", path);
            return std::nullopt;
        }
        
        if (auto it = s_textures.find(resourceName); it != s_textures.end())
        {
            ZN_CORE_ERROR("[ResourceManager::LoadTexture] Failed to load shader. Shader ({}) resource already exists in the registry", resourceName);
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
            
            SharedPtr<Texture> texture = CreateShared<Texture>(data, width, height, channels, internalFormat, dataFormat);
            s_textures[resourceName] = texture;
            
            stbi_image_free(data);

            return texture;
        }
        
        ZN_CORE_ERROR("[ResourceManager::LoadTexture] Failed to load texture resource. Library (stbi) failed to load texture: {}", path);
        return std::nullopt;
    }

    ResourceManager::TextureResource ResourceManager::GetTexture(const String& name)
    {
        auto it = s_textures.find(name);
        if (it == s_textures.end())
        {
            ZN_CORE_ERROR("[ResourceManager::GetTexture] Failed to retrieve texture. Texture ({}) resource doesn't exist in the registry", name);
            return std::nullopt;
        }
        
        return it->second;
    }

    void ResourceManager::Shutdown()
    {
        
    }
}
