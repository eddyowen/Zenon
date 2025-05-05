#pragma once

#include "Core/Assert.hpp"
#include "Core/Base.hpp"
#include "Core/Log.hpp"
#include "Input/KeyCodes.hpp"

#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

namespace zn
{

    template<typename T>
    class Handle
    {
    public:
        using ResourceType = T;
        
        static constexpr u32 GEN_SHIFT = 32;
        static constexpr u32 INDEX_MASK = 0xFFFFFFFF;
        static constexpr u32 MAX_INDEX_VALUE = 0xFFFFFFFF;
        
        Handle(u32 index, u32 generation)
            : m_value(static_cast<u64>(generation) << GEN_SHIFT | index){}

        u32 GetIndex() const { return static_cast<u32>(m_value & INDEX_MASK); }
        u32 GetGeneration() const { return static_cast<u32>(m_value >> GEN_SHIFT); }
        
    private:
        u64 m_value = 0;
    };

    template<typename T>
    class ResourceRegistry
    {
    public:
        struct Entry
        {
            T Data;
            u32 Generation = 0;
            b8 IsActive;
        };
        
        ResourceRegistry() { m_freeIndices.reserve(10); m_entries.reserve(10); }
        ~ResourceRegistry() = default;
        
        ResourceRegistry(const ResourceRegistry& other) = delete;
        ResourceRegistry(ResourceRegistry&& other) noexcept = delete;
        
        ResourceRegistry& operator=(const ResourceRegistry& other) = delete;
        ResourceRegistry& operator=(ResourceRegistry&& other) noexcept = delete;

        
        Opt<Handle<T>> CreateResource(const T& resource)
        {
            return AddResourceInternal(resource);
        }

        Opt<Handle<T>> CreateResource(T&& resource)
        {
            return AddResourceInternal(std::move(resource));
        }

        template<typename... Args>
        Opt<Handle<T>> EmplaceResource(Args&&... args)
        {
            ZN_CORE_TRACE("EMPLACING BACK");
            
            if (!m_freeIndices.empty())
            {
                return ReuseSlot(T{std::forward<Args>(args)...});
            }

            if (m_entries.size() > Handle<T>::MAX_INDEX_VALUE)
            {
                return std::nullopt;
            }

            u32 index = static_cast<u32>(m_entries.size());
            m_entries.emplace_back(Entry{T{std::forward<Args>(args)...}, 0, true});

            return Handle<T>{index, 0};
        }

        void ReleaseResource(Handle<T> handle)
        {
            if (Entry* entry = ValidateHandle(handle))
            {
                ZN_CORE_TRACE("RELEASING RESOURCE");
                
                entry->IsActive = false;

                ++entry->Generation;
                // If this is the case, the generation counter probably wrapped around,
                // maybe I should take care of this case. But for now it's probably unlikely, as
                // I'm using a u32 for it
                ZN_ASSERT(entry->Generation != 0, "Generation counter wrapped around!");

                m_freeIndices.push_back(handle.GetIndex());
            }
        }

    private:
        template<typename U>
        Opt<Handle<U>> AddResourceInternal(U&& resource)
        {
            // The free indices list contain data only when items are released
            if (!m_freeIndices.empty())
            {
                ReuseSlot(std::forward<U>(resource));
            }

            return CreateNewInternal(std::forward<U>(resource));
        }

        template<typename U>
        Opt<Handle<U>> ReuseSlot(U&& resource)
        {
            ZN_CORE_TRACE("REUSED SLOT");
            
            u32 index = m_freeIndices.back();

            m_freeIndices.pop_back();

            Entry& entry = m_entries[index];
            entry.Data = std::forward<U>(resource);
            entry.IsActive = true;

            return Handle<U>(index, entry.Generation);
        }

        template<typename U>
        Opt<Handle<U>> CreateNewInternal(U&& resource)
        {
            if (m_entries.size() > Handle<U>::MAX_INDEX_VALUE)
            {
                return std::nullopt;
            }

            ZN_CORE_TRACE("CREATE NEW RESOURCE");

            u32 index = static_cast<u32>(m_entries.size());
            m_entries.emplace_back(std::forward<U>(resource), 0, true);
            
            return Handle<U>{index, 0};
        }

        const Entry* ValidateHandle(const Handle<T> handle) const
        {
            const u32 index = handle.GetIndex();
            if (index >= m_entries.size())
            {
                return nullptr;
            }
            
            const Entry& entry = m_entries[(handle.GetIndex())];
            
            return entry.IsActive && entry.Generation == handle.GetGeneration() ? &entry : nullptr;
        }

        Entry* ValidateHandle(Handle<T> handle)
        {
            const u32 index = handle.GetIndex();
            if (index >= m_entries.size())
            {
                return nullptr;
            }
            
            Entry& entry = m_entries[(handle.GetIndex())];
            
            return entry.IsActive && entry.Generation == handle.GetGeneration() ? &entry : nullptr;
        }
        
        Vector<Entry> m_entries;
        Vector<u32> m_freeIndices;
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
