#pragma once

#include "Core/Assert.hpp"
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

        Handle() = default;
        
        explicit constexpr Handle(IndexType index, GenerationType generation)
            : m_value(static_cast<ValueType>(generation) << GEN_SHIFT | index){}

        [[nodiscard]] constexpr IndexType GetIndex() const { return static_cast<IndexType>(m_value & INDEX_MASK); }
        [[nodiscard]] constexpr GenerationType GetGeneration() const { return static_cast<GenerationType>(m_value >> GEN_SHIFT); }
        
    private:
        ValueType m_value = 0;
    };

    template<Moveable T>
    class ResourceRegistry
    {
    public:
        using IndexType = typename Handle<T>::IndexType;
        using GenerationType = typename Handle<T>::GenerationType;
        
        struct Entry
        {
            T Data;
            GenerationType Generation = 0;
            b8 IsActive = false;

            // To be able to "perfect forward" args to T Data. See -> EmplaceResource
            template<typename... TArgs>
            explicit constexpr Entry(TArgs&&... args)
                : Data(std::forward<TArgs>(args)...), Generation(0), IsActive(true)
            {
                static_assert(ConstructibleWithArgs<T, TArgs...>, "ResourceRegistry::Entry inner type cannot be constructed with the provided arguments");
            }
        };
        
        ResourceRegistry() = default;
        ~ResourceRegistry() = default;
        
        ResourceRegistry(const ResourceRegistry& other) = delete;
        ResourceRegistry(ResourceRegistry&& other) noexcept = delete;
        
        ResourceRegistry& operator=(const ResourceRegistry& other) = delete;
        ResourceRegistry& operator=(ResourceRegistry&& other) noexcept = delete;

        void InitStorage(uSize newCapacity) { m_entries.reserve(newCapacity); m_freeIndices.reserve(newCapacity); }

        [[nodiscard]] Opt<Handle<T>> CreateResource(const T& resource)
        {
            return AddResourceInternal(resource);
        }

        [[nodiscard]] Opt<Handle<T>> CreateResource(T&& resource)
        {
            return AddResourceInternal(std::forward<T>(resource));
        }

        template<typename... Args>
        requires ConstructibleWithArgs<T, Args...>
        [[nodiscard]] Opt<Handle<T>> EmplaceResource(Args&&... args)
        {
            if (!m_freeIndices.empty())
            {
                return ReuseSlot<Args...>(std::forward<Args>(args)...);
            }

            // TODO: Think about what to do in this situation... but very unlikely lol
            ZN_ASSERT(m_entries.size() < Handle<T>::MAX_INDEX_VALUE);

            const IndexType index = static_cast<IndexType>(m_entries.size());
            m_entries.emplace_back(std::forward<Args>(args)...);

            return Handle<T>{index, 0};
        }

        bool ReleaseResource(Handle<T> handle)
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

                return true;
            }

            return false;
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
        requires CallableWithArgs<F, T&>
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

        template<typename F>
        requires MatchingSignature<F, const T&, void>
        void ForEachActiveResource(F&& func) const
        {
            for (IndexType i = 0; i < m_entries.size(); ++i)
            {
                const Entry* entry = &m_entries[i];
                if (entry->IsActive)
                {
                    Handle<T> handle = Handle<T>{i, entry->Generation};
                    func(entry->Data);
                }
            }
        }

        template<typename F>
        requires MatchingSignature<F, T&, void>
        void ForEachActiveResource(F&& func)
        {
            for (IndexType i = 0; i < m_entries.size(); ++i)
            {
                Entry* entry = &m_entries[i];
                if (entry->IsActive)
                {
                    Handle<T> handle = Handle<T>{i, entry->Generation};
                    func(entry->Data);
                }
            }
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
        [[nodiscard]] Opt<Handle<T>> AddResourceInternal(T&& resource)
        {
            // The free indices list contains data only when items are released
            if (!m_freeIndices.empty())
            {
                return ReuseSlot(std::forward<T>(resource));
            }

            return CreateNewInternal(std::forward<T>(resource));
        }

        [[nodiscard]] Opt<Handle<T>> ReuseSlot(T&& resource)
        {
            const IndexType index = m_freeIndices.back();
            m_freeIndices.pop_back();

            // [WARNING] Assumes the T stored inside the slot has been "destroyed" previously
            Entry& entry = m_entries[index];
            std::construct_at(std::addressof(entry.Data), std::forward<T>(resource));
            entry.IsActive = true;

            return Handle<T>(index, entry.Generation);
        }

        template<typename... TArgs>
        requires ConstructibleWithArgs<T, TArgs...>
        [[nodiscard]] Opt<Handle<T>> ReuseSlot(TArgs&&... targs)
        {
            const IndexType index = m_freeIndices.back();
            m_freeIndices.pop_back();

            // Assumes the T stored inside the slot has been "destroyed" previously
            Entry& entry = m_entries[index];
            std::construct_at(std::addressof(entry.Data), std::forward<TArgs>(targs)...);
            entry.IsActive = true;

            return Handle<T>(index, entry.Generation);
        }

        [[nodiscard]] Opt<Handle<T>> CreateNewInternal(T&& resource)
        {
            if (m_entries.size() > Handle<T>::MAX_INDEX_VALUE)
            {
                return std::nullopt;
            }

            const IndexType index = static_cast<IndexType>(m_entries.size());
            m_entries.emplace_back(std::forward<T>(resource), 0, true);
            
            return Handle<T>{index, 0};
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
