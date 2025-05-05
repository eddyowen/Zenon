#pragma once

#include "Core/Log.hpp"

#include <string_view>

namespace zn
{
    class Lifetime 
    {
    public:
        // Constructor
        Lifetime(std::string_view name) : m_instanceName(name), m_instanceId(++s_objectCount) 
        {
            ZN_CORE_TRACE("Constructor called for {} (ID: {})", m_instanceName, m_instanceId);
        }

        // Copy constructor
        Lifetime(const Lifetime& other) : m_instanceName(other.m_instanceName), m_instanceId(++s_objectCount) 
        {
            ZN_CORE_TRACE("Copy constructor called for {} (ID: {})", m_instanceName, m_instanceId);
        }

        // Move constructor
        Lifetime(Lifetime&& other) noexcept : m_instanceName(std::move(other.m_instanceName)), m_instanceId(++s_objectCount) 
        {
            ZN_CORE_TRACE("Move constructor called for {} (ID: {})", m_instanceName, m_instanceId);
        }

        // Copy assignment operator
        Lifetime& operator=(const Lifetime& other) 
        {
            if (this == &other) 
                return *this; // Handle self-assignment
        
            m_instanceName = other.m_instanceName;
        
            // Note: We choose not to change the ID on assignment to preserve the identity of the original object
            ZN_CORE_TRACE("Copy assigment operator called for {} (ID: {})", m_instanceName, m_instanceId);
            return *this;
        }

        // Move assignment operator
        Lifetime& operator=(Lifetime&& other) noexcept 
        {
            if (this == &other) 
                return *this; // Handle self-assignment
        
            m_instanceName = std::move(other.m_instanceName);

            // Note: We choose not to change the ID on assignment to preserve the identity of the original object
            ZN_CORE_TRACE("Move assignment operator called for {} (ID: {})", m_instanceName, m_instanceId);
            return *this;
        }

        // Destructor
        ~Lifetime() 
        {
            ZN_CORE_TRACE("Destructor called for {} (ID: {})", m_instanceName, m_instanceId);
        }

        // A method to demonstrate changing the instance's name
        void rename(std::string_view newName) 
        {
            m_instanceName = newName;
            ZN_CORE_TRACE("Instance renamed to {} (ID: {})", m_instanceName, m_instanceId);
        }

        // Static method to get the current object count
        static int gets_objectCount() 
        {
            return s_objectCount;
        }

    private:
        static inline int s_objectCount = 0; // Inline variable for simplicity
        std::string_view m_instanceName;
        int m_instanceId;
    };
}
