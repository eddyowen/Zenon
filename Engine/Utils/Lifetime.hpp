#pragma once

#include "Core/Log.hpp"

#include <string_view>

namespace zn
{
    class Lifetime 
    {
    public:
        // Constructor
        Lifetime(StringView name, bool enableLogs)
            : m_instanceName(name), m_instanceId(++s_objectCount), m_enableLogs(enableLogs)
        {
            LogFunctionCall("Constructor");
        }

        // Copy constructor
        Lifetime(const Lifetime& other)
            : m_instanceName(other.m_instanceName), m_instanceId(++s_objectCount), m_enableLogs(other.m_enableLogs)
        {
            LogFunctionCall("Copy constructor");
        }

        // Move constructor
        Lifetime(Lifetime&& other) noexcept
            : m_instanceName(std::move(other.m_instanceName)), m_instanceId(++s_objectCount), m_enableLogs(other.m_enableLogs)
        {
            LogFunctionCall("Move constructor");
        }

        // Copy assignment operator
        Lifetime& operator=(const Lifetime& other) 
        {
            if (this == &other) 
                return *this; // Handle self-assignment
        
            m_instanceName = other.m_instanceName;
            m_enableLogs = other.m_enableLogs;
        
            // Note: We choose not to change the ID on assignment to preserve the identity of the original object
            LogFunctionCall("Copy assignment operator");
            return *this;
        }

        // Move assignment operator
        Lifetime& operator=(Lifetime&& other) noexcept 
        {
            if (this == &other) 
                return *this; // Handle self-assignment
        
            m_instanceName = std::move(other.m_instanceName);
            m_enableLogs = other.m_enableLogs;

            // Note: We choose not to change the ID on assignment to preserve the identity of the original object
            LogFunctionCall("Move assignment operator");
            return *this;
        }

        // Destructor
        ~Lifetime() 
        {
            LogFunctionCall("Destructor");
        }

        // A method to demonstrate changing the instance's name
        void Rename(StringView newName) 
        {
            m_instanceName = newName;
            LogFunctionCall("Rename");
        }

        void LogFunctionCall(StringView functionName)
        {
            if (m_enableLogs)
                ZN_CORE_TRACE("{} called for {} (ID: {})", functionName, m_instanceName, m_instanceId);
        }

        static u32 GetObjectCount() { return s_objectCount; }
        StringView GetName() const { return m_instanceName; }

    private:
        static inline u32 s_objectCount = 0; // Inline variable for simplicity
        
        StringView m_instanceName;
        u32 m_instanceId;
        bool m_enableLogs = false;
    };
}
