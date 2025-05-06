#pragma once

#include "Core/Base.hpp"

namespace zn
{
    template<typename T>
    class Handle
    {
    public:
        using ResourceType = T;
        using IndexType = u32;
        using GenerationType = u32;
        using ValueType = u64;
        
        static constexpr u32 GEN_SHIFT = 32;
        static constexpr u32 INDEX_MASK = 0xFFFFFFFF;
        static constexpr u32 MAX_INDEX_VALUE = 0xFFFFFFFF;
        
        explicit constexpr Handle(IndexType index, GenerationType generation)
            : m_value(static_cast<ValueType>(generation) << GEN_SHIFT | index){}

        [[nodiscard]] constexpr IndexType GetIndex() const { return static_cast<IndexType>(m_value & INDEX_MASK); }
        [[nodiscard]] constexpr GenerationType GetGeneration() const { return static_cast<GenerationType>(m_value >> GEN_SHIFT); }
        
    private:
        ValueType m_value = 0;
    };

    template<typename T>
    class ResourceRegistry
    {
    public:
        using IndexType = typename Handle<T>::IndexType;
        using GenerationType = typename Handle<T>::GenerationType;
        
        struct Entry
        {
            T Data;
            GenerationType Generation = 0;
            b8 IsActive;
        };
        
        ResourceRegistry() = default;
        ~ResourceRegistry() = default;
        
        ResourceRegistry(const ResourceRegistry& other) = delete;
        ResourceRegistry(ResourceRegistry&& other) noexcept = delete;
        
        ResourceRegistry& operator=(const ResourceRegistry& other) = delete;
        ResourceRegistry& operator=(ResourceRegistry&& other) noexcept = delete;

        [[nodiscard]] Opt<Handle<T>> CreateResource(const T& resource)
        {
            return AddResourceInternal(resource);
        }

        [[nodiscard]] Opt<Handle<T>> CreateResource(T&& resource)
        {
            return AddResourceInternal(std::move(resource));
        }

        template<typename... Args>
        [[nodiscard]] Opt<Handle<T>> EmplaceResource(Args&&... args)
        {
            if (!m_freeIndices.empty())
            {
                return ReuseSlot(T{std::forward<Args>(args)...});
            }

            if (m_entries.size() > Handle<T>::MAX_INDEX_VALUE)
            {
                return std::nullopt;
            }

            const IndexType index = static_cast<IndexType>(m_entries.size());
            m_entries.emplace_back(Entry{T{std::forward<Args>(args)...}, 0, true});

            return Handle<T>{index, 0};
        }

        void ReleaseResource(Handle<T> handle)
        {
            if (Entry* entry = ValidateHandle(handle))
            {
                if constexpr (!std::is_trivially_destructible_v<T>)
                {
                    entry->Data.~T();
                }
                
                entry->IsActive = false;
                
                ++entry->Generation;
                // If this is the case, the generation counter probably wrapped around,
                // maybe I should take care of this case. But for now it's probably unlikely, as
                // I'm using a u32 for it
                ZN_ASSERT(entry->Generation != 0, "Generation counter wrapped around!");

                m_freeIndices.push_back(handle.GetIndex());
            }
        }

        [[nodiscard]] Opt<CRefWrapper<T>> GetResourceRef(Handle<T> handle) const
        {
            if (const Entry* entry = ValidateHandle(handle))
            {
                return std::cref(entry->Data);
            }

            return std::nullopt;
        }

        template<typename F>
        requires std::is_invocable_v<F, T&>
        b8 ModifyResource(Handle<T> handle, F&& modifyFunc)
        {
            if (Entry* entry = ValidateHandle(handle))
            {
                if (entry->IsActive)
                {
                    modifyFunc(entry->Data);
                    return true;
                }
            }
            
            return false;
        }

        [[nodiscard]] uSize GetTotalSlotsCount() const
        {
            return m_entries.size();    
        }

        [[nodiscard]] uSize GetFreeSlotsCount() const
        {
            return m_freeIndices.size();    
        }

        [[nodiscard]] uSize GetSlotsInUseCount() const
        {
            uSize total = 0;
            for (auto& entry : m_entries)
            {
                if (entry.IsActive)
                {
                    total++;
                }
            }

            return total;
        }

    private:
        template<typename U>
        [[nodiscard]] Opt<Handle<U>> AddResourceInternal(U&& resource)
        {
            // The free indices list contain data only when items are released
            if (!m_freeIndices.empty())
            {
                return ReuseSlot(std::forward<U>(resource));
            }

            return CreateNewInternal(std::forward<U>(resource));
        }

        template<typename U>
        [[nodiscard]] Opt<Handle<U>> ReuseSlot(U&& resource)
        {
            const IndexType index = m_freeIndices.back();
            m_freeIndices.pop_back();

            Entry& entry = m_entries[index];
            entry.Data = std::forward<U>(resource);
            entry.IsActive = true;

            return Handle<U>(index, entry.Generation);
        }

        template<typename U>
        [[nodiscard]] Opt<Handle<U>> CreateNewInternal(U&& resource)
        {
            if (m_entries.size() > Handle<U>::MAX_INDEX_VALUE)
            {
                return std::nullopt;
            }

            const IndexType index = static_cast<IndexType>(m_entries.size());
            m_entries.emplace_back(std::forward<U>(resource), 0, true);
            
            return Handle<U>{index, 0};
        }

        [[nodiscard]] const Entry* ValidateHandle(const Handle<T> handle) const
        {
            const IndexType index = handle.GetIndex();
            if (index >= m_entries.size())
            {
                return nullptr;
            }
            
            const Entry& entry = m_entries[index];
            
            return entry.IsActive && entry.Generation == handle.GetGeneration() ? &entry : nullptr;
        }

        [[nodiscard]] Entry* ValidateHandle(Handle<T> handle)
        {
            const IndexType index = handle.GetIndex();
            if (index >= m_entries.size())
            {
                return nullptr;
            }
            
            Entry& entry = m_entries[index];
            
            return entry.IsActive && entry.Generation == handle.GetGeneration() ? &entry : nullptr;
        }
        
        Vector<Entry> m_entries;
        Vector<IndexType> m_freeIndices;
    };
}
