#pragma once

#include "Log.h"

#include <functional>
#include <map>

namespace zn
{
    struct EventHandler
    {
        static constexpr std::size_t InvalidId = static_cast<std::size_t>(-1);

        EventHandler() : Id(InvalidId) {}
        EventHandler(const std::size_t id) : Id(id) {}

        bool operator==(const EventHandler& other) const { return Id == other.Id; }
        
        std::size_t Id;
    };

    template <typename... Args>
    class Signal {
    public:
        using slot_type = std::function<void(Args...)>;

        Signal() : m_currentId(EventHandler::InvalidId) {}
        ~Signal() { m_slots.clear(); m_currentId = EventHandler::InvalidId; }

        // Connects to a free function or a lambda
        EventHandler Connect(slot_type&& slot)
        {
            auto id = m_currentId++;
            m_slots[id] = std::move(slot);
            
            return id;
        }

        // Connect a member function. Forcing to receive a reference, instead of a pointer
        // so the user can't do e.g:
        //
        //      Foo* foo = new Foo();
        //      exampleSignal.connect(foo, &Application::ExampleFunction)
        //
        // It would cause UB if the object is deleted and Signal is called.
        template <typename T>
        EventHandler Connect(T& instance, void(T::*memberFunc)(Args...))
        {
            return Connect([&instance, memberFunc](Args... args){ (instance.*memberFunc)(args...); });
        }

        template <typename T>
        EventHandler Connect(const T& instance, void(T::*memberFunc)(Args...) const)
        {
            return Connect([&instance, memberFunc](Args... args){ (instance.*memberFunc)(args...); });
        }
        
        // Emit the signal, calling all connected slots
        void Emit(Args... args) const
        {
            for (const auto& [handler, slot] : m_slots)
            {
                if (slot)
                {
                    slot(args...);
                }
            }
        }
        
        // Disconnect a slot using its connection ID
        void Disconnect(std::size_t id)
        {
            ZN_CORE_ASSERT(id != EventHandler::InvalidId)
            m_slots.erase(id);
        }

        void DisconnectAll()
        {
            m_slots.clear();
        }

    private:
        std::unordered_map<EventHandler, slot_type> m_slots;
        std::size_t m_currentId;
    };
}

namespace std {
    template <>
    struct hash<zn::EventHandler> {
        std::size_t operator()(const zn::EventHandler& handler) const noexcept {
            return std::hash<std::size_t>()(handler.Id);
        }
    };
}
