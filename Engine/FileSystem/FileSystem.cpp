#include "FileSystem.h"

#include <fstream>

namespace zn
{
    std::string FileSystem::PathNormalizer::Normalize(std::string_view path)
    {
        std::string normalized(path);
        
        // Convert backslashes to forward slashes
        std::ranges::replace(normalized, '\\', '/');
        
        // Normalize path using filesystem library
        Path fileSytemPath(normalized);
        fileSytemPath = fileSytemPath.lexically_normal();
        
        // Convert to generic string format
        normalized = fileSytemPath.generic_string();
        
        return normalized;
    }

    bool FileSystem::Exists(const std::string& path)
    {
        try
        {
            if (auto fullPath = GetFullPath(path))
            {
                return std::filesystem::exists(fullPath.value());
            }
            else
            {
                return false;
            }
        } catch (...)
        {
            return false;
        }
    }

    bool FileSystem::IsFile(const std::string& path)
    {
        try
        {
            if (auto filesystemPath = GetFullPath(path))
            {
                return std::filesystem::is_regular_file(filesystemPath.value());
            }
            
            return false;
            
        } catch (...)
        {
            return false;
        }
    }

    bool FileSystem::IsDirectory(const std::string& path)
    {
        try
        {
            if (auto filesystemPath = GetFullPath(path))
            {
                return std::filesystem::is_directory(filesystemPath.value());
            }
                
            return false;
                
        } catch (...)
        {
            return false;
        }
    }

    std::vector<std::string> FileSystem::ListDirectory(const std::string& path)
    {
        std::vector<std::string> entries;
            
        if(auto fullPath = GetFullPath(path))
        {
            if (!std::filesystem::is_directory(fullPath.value()))
            {
                return entries;
            }

            for (const DirectoryEntry& entry : std::filesystem::directory_iterator(fullPath.value()))
            {
                try
                {
                    Path relativePath = entry.path().lexically_relative(GetRoot());
                    entries.push_back(relativePath.generic_string());
                } catch (...)
                {
                    // Skip invalid entries
                }
            }

            return entries;
        }

        ZN_CORE_WARN("[FileSystem::ListDirectory] Could not list directory. Returning empty list")
        return {};
    }

    std::optional<std::vector<std::byte>> FileSystem::ReadFileAsBinary(const std::string& path)
    {
        try
        {
            if (auto fullPath = GetFullPath(path))
            {
                std::ifstream file(fullPath.value(), std::ios::binary | std::ios::ate);
                if (!file.is_open())
                {
                    ZN_CORE_ERROR("[FileSystem::ReadFileAsBinary] Failed to open file at '" + path + "'")
                    return std::nullopt;
                }

                std::streamsize size = file.tellg();
                file.seekg(0, std::ios::beg);

                std::vector<std::byte> buffer(size);
                if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
                {
                    ZN_CORE_ERROR("[FileSystem::ReadFileAsBinary] Failed to read file at '" + path + "'")
                    return std::nullopt;
                }

                return buffer;   
            }
            else
            {
                ZN_CORE_ERROR("[FileSystem::ReadFileAsBinary] Failed to read file at '" + path + "'")
                return std::nullopt;
            }
        } catch (...)
        {
            return std::nullopt;
        }
    }

    std::optional<std::string> FileSystem::ReadFileAsString(const std::string& path)
    {
        try
        {
            if (auto fullPath = GetFullPath(path))
            {
                std::ifstream file(fullPath.value(), std::ios::binary | std::ios::ate);
                if (!file.is_open())
                {
                    ZN_CORE_ERROR("[FileSystem::ReadFileAsString] Failed to open file at '" + path + "'")
                    return std::nullopt;
                }

                const std::streamsize size = file.tellg();
                file.seekg(0, std::ios::beg);

                std::string buffer;
                buffer.resize(static_cast<size_t>(size));
            
                if (!file.read(buffer.data(), size))
                {
                    ZN_CORE_ERROR("[FileSystem::ReadFileAsString] Failed to read file at '" + path + "'")
                    return std::nullopt;
                }

                return buffer;   
            }
            else
            {
                ZN_CORE_ERROR("[FileSystem::ReadFileAsString] Failed to read file at '" + path + "'")
                return std::nullopt;
            }
        } catch (...)
        {
            return std::nullopt;
        }
    }

    std::optional<FileSystem::Path> FileSystem::GetFullPath(const std::string& path)
    {
        std::string normalized = PathNormalizer::Normalize(path);
        Path fileSystemPath(normalized);
            
        if (fileSystemPath.is_absolute())
        {
            ZN_CORE_ERROR("[FileSystem::GetFullPath] Absolute paths not allowed: " + normalized)
            return std::nullopt;
        }
            
        Path fullPath = (GetRoot()/fileSystemPath).lexically_normal();

        // Security check: prevent directory traversal
        Path relativeToRoot = fullPath.lexically_relative(GetRoot());
        if (relativeToRoot.generic_string().find("..") != std::string::npos)
        {
            ZN_CORE_ERROR("[FileSystem::GetFullPath] Path escapes root directory: " + normalized)
            return std::nullopt;
        }

        return fullPath;
    }
}
