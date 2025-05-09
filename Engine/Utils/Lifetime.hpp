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

    // TestAssert: A simple assertion macro for tests
    #define TEST_ASSERT(condition, message) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed (" #condition "): " + std::string(message)); \
    }

    // TestResource: A helper class to test resource management, tracking constructions/destructions.
    struct TestResource
    {
        int id;
        std::string data;

        // Static counters for tracking operations
        static int ctor_default_count; // Default constructor
        static int ctor_value_count;   // Constructor with id and data
        static int ctor_copy_count;    // Copy constructor
        static int ctor_move_count;    // Move constructor
        static int dtor_count;         // Destructor
        static int copy_assign_count;  // Copy assignment
        static int move_assign_count;  // Move assignment


        // Default constructor
        TestResource() : id(-1), data("default_constructed")
        {
            ctor_default_count++;
        }

        // Constructor with parameters
        TestResource(int i, std::string d = "default_data") : id(i), data(std::move(d))
        {
            ctor_value_count++;
        }

        // Copy constructor
        TestResource(const TestResource& other) : id(other.id), data(other.data)
        {
            ctor_copy_count++;
        }

        // Move constructor
        TestResource(TestResource&& other) noexcept : id(other.id), data(std::move(other.data))
        {
            ctor_move_count++;
            other.id = -2; // Mark as moved-from
            other.data = "moved_from_ctor";
        }

        // Destructor
        ~TestResource() {
            dtor_count++;
        }

        // Copy assignment operator
        TestResource& operator=(const TestResource& other)
        {
            if (this != &other) {
                id = other.id;
                data = other.data;
                copy_assign_count++;
            }
            return *this;
        }

        // Move assignment operator
        TestResource& operator=(TestResource&& other) noexcept
        {
            if (this != &other)
            {
                id = other.id;
                data = std::move(other.data);
                other.id = -3; // Mark as moved-from
                other.data = "moved_from_assign";
                move_assign_count++;
            }
            return *this;
        }
        
        // Static method to reset all counters, typically called before each test case
        static void ResetCounters()
        {
            ctor_default_count = 0;
            ctor_value_count = 0;
            ctor_copy_count = 0;
            ctor_move_count = 0;
            dtor_count = 0;
            copy_assign_count = 0;
            move_assign_count = 0;
        }

        // Equality operator for comparisons in tests
        bool operator==(const TestResource& other) const {
            return id == other.id && data == other.data;
        }

        // For printing TestResource objects, useful for debugging
        friend std::ostream& operator<<(std::ostream& os, const TestResource& tr)
        {
            os << "TestResource{id=" << tr.id << ", data=\"" << tr.data << "\"}";
            return os;
        }
    };

    // Initialize static member variables of TestResource
    int TestResource::ctor_default_count = 0;
    int TestResource::ctor_value_count = 0;
    int TestResource::ctor_copy_count = 0;
    int TestResource::ctor_move_count = 0;
    int TestResource::dtor_count = 0;
    int TestResource::copy_assign_count = 0;
    int TestResource::move_assign_count = 0;
}
