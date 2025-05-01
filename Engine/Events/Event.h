#pragma once

#include "Core/Base.h"

#include <typeindex>
#include <any>
#include <algorithm>
#include <concepts> 

namespace zn
{
	template <typename EventT>
	class Connection 
	{
	public:
		using HandlerId = u64;
		
	    Connection(class EventSystem* eventSystem, HandlerId id)
	        : m_eventSystem(eventSystem), m_id(id) {}

		~Connection() { Disconnect(); }
		
		Connection(const Connection& other) = delete;
		Connection(Connection&& other) noexcept = delete;
		
		Connection& operator=(const Connection& other) = delete;
		Connection& operator=(Connection&& other) noexcept = delete;

	    void Disconnect() 
	    {
	        if (m_eventSystem) 
	        {
	            m_eventSystem->template Unsubscribe<EventT>(m_id);
	            m_eventSystem = nullptr;
	        }
	    }

	    bool Connected() const { return m_eventSystem != nullptr; }
	    HandlerId GetId() const { return m_id; }
	    
	private:
	    EventSystem* m_eventSystem;
	    HandlerId m_id;
	};

	template<typename EventT>
	using EventConnection = SharedPtr<Connection<EventT>>;

	class EventSystem 
	{
	public:
	    using HandlerId = u64;

		static EventSystem& Instance()
		{
			static EventSystem instance;
			return instance;
		}

		EventSystem() = default;
		~EventSystem() = default;
		
		EventSystem(const EventSystem&) = delete;
		EventSystem(EventSystem&&) = delete;
		
		EventSystem& operator=(const EventSystem&) = delete;
		EventSystem& operator=(EventSystem&&) = delete; 
	    
	    template<typename EventT, typename Callable, typename FilterCallable = std::nullptr_t>
	    requires std::invocable<Callable, const EventT&> && std::same_as<std::invoke_result_t<Callable, const EventT&>, bool>
		[[nodiscard]]
	    SharedPtr<Connection<EventT>> Subscribe(Callable&& callback, int priority = 0, FilterCallable&& filter = nullptr)
	    {
	        Func<bool(const EventT&)> func = std::forward<Callable>(callback);
	    	
	        Func<bool(const EventT&)> filterFunc;
	        if constexpr (!std::is_same_v<FilterCallable, std::nullptr_t>) 
	        {
	            filterFunc = std::forward<FilterCallable>(filter);
	        }

	        HandlerId id = m_nextHandlerId++;
	        auto& list = GetHandlerList<EventT>();
	        HandlerEntry<EventT> entry{ id, priority, std::move(func), std::move(filterFunc) };

	        auto it = std::lower_bound(list.begin(), list.end(), entry,
	        [](const HandlerEntry<EventT>& a, const HandlerEntry<EventT>& b) 
	        {
	            return a.Priority > b.Priority;
	        });

	        list.insert(it, std::move(entry));
	        return CreateShared<Connection<EventT>>(this, id);
	    }

	    template<typename EventT, typename T, typename MemFn>
	    requires std::is_member_function_pointer_v<MemFn>
		[[nodiscard]]
	    SharedPtr<Connection<EventT>> Subscribe(SharedPtr<T> instance, MemFn memFn, int priority = 0, Func<bool(const EventT&)> filter = nullptr)
	    {
	        WeakPtr<T> weakInstance = instance;
	        auto callback = [weakInstance, memFn](const EventT& event) -> bool
	        {
	            if (auto strongInstance = weakInstance.lock()) 
	            {
	                return (strongInstance.get()->*memFn)(event);
	            }

	            return false;
	        };
	    	
	        return Subscribe<EventT>(std::move(callback), priority, std::move(filter));
	    }
	    
	    template<typename EventT>
	    void Unsubscribe(HandlerId id) 
	    {
	        auto& list = GetHandlerList<EventT>();
	        
	        auto it = std::remove_if(list.begin(), list.end(),
	            [id](const HandlerEntry<EventT>& entry) { return entry.Id == id; });
	        
	        list.erase(it, list.end());
	    }
	    
	    template<typename EventT>
	    void Post(const EventT& event)
	    {
	        const auto handlers = GetHandlerList<EventT>();  // Safe copy for iteration
	        for (const auto& handler : handlers) 
	        {
	            if (handler.Filter && !handler.Filter(event))
	                continue;
	            
	            if (handler.Callback(event))
	                break;
	        }
	    }
	    
	private:
	    template<typename EventT>
	    struct HandlerEntry 
	    {
	        HandlerId Id;
	        i64 Priority;
	        Func<bool(const EventT&)> Callback;
	        Func<bool(const EventT&)> Filter;
	    };
	    
	    template<typename EventT>
	    Vector<HandlerEntry<EventT>>& GetHandlerList() 
	    {
	        std::type_index key(typeid(EventT));
	        
	        auto it = m_handlerMap.find(key);
	        if (it == m_handlerMap.end())
	        {
	            it = m_handlerMap.try_emplace(key, Vector<HandlerEntry<EventT>>{}).first;
	        }
	        
	        return *std::any_cast<Vector<HandlerEntry<EventT>>>(&it->second);
	    }
	    
	    UMap<std::type_index, std::any> m_handlerMap;
	    HandlerId m_nextHandlerId = 0;
	};
}


//// --- Lambda-based subscription ---
//auto connLambda = dispatcher.Subscribe<NewKeyEvent>(
//    [](const NewKeyEvent& event) -> bool {
//        ZN_CORE_INFO("[Lambda] KeyEvent: key = {}", event.key)
//        if (event.key == 42) {
//            ZN_CORE_INFO("Consumed by lambda handler.")
//            return true;
//        }
//        return false;
//    },
//    10  // high priority
//);
//
//// --- Free function subscription ---
//auto connFreeFunc = dispatcher.Subscribe<NewKeyEvent>(freeKeyHandler, 5);
//
//// --- Member function subscription ---
//std::shared_ptr<KeyHandler> keyHandlerInstance= std::make_shared<KeyHandler>();
//auto connMember = dispatcher.Subscribe<NewKeyEvent>(app, &Application::handleKey, 7);
//
//// --- Const member function subscription ---
//const  std::shared_ptr<KeyHandler> constKeyHandlerInstance= std::make_shared<KeyHandler>();
//auto connMemberConst = dispatcher.Subscribe<NewKeyEvent>(constKeyHandlerInstance, &KeyHandler::handleKeyConst, 6);
//
//// --- Subscription with filter ---
//// For MouseMoveEvent, process only if x > 100.
//auto connMouse = dispatcher.Subscribe<NewMouseMoveEvent>(
//    [](const NewMouseMoveEvent& event) -> bool {
//        ZN_CORE_INFO("[Lambda] MouseMoveEvent: x = {}, y = {}", event.x, event.y)
//        return false;
//    },
//    0,  // default priority
//    [](const NewMouseMoveEvent& event) -> bool {
//        return event.x > 100;
//    }
//);
//
//// --- Post events ---
//ZN_CORE_INFO("Posting KeyEvent with key = 13...")
//NewKeyEvent keyEvent1 { 13, 0, 1, 0 };
//dispatcher.Post(keyEvent1);
//
//ZN_CORE_INFO("\nPosting KeyEvent with key = 42...")
//NewKeyEvent keyEvent2 { 42, 0, 1, 0 };
//dispatcher.Post(keyEvent2);
//
//ZN_CORE_INFO("\nPosting MouseMoveEvent with x = 50...")
//NewMouseMoveEvent mouseEvent1 { 50, 200 };
//dispatcher.Post(mouseEvent1);
//
//ZN_CORE_INFO("\nPosting MouseMoveEvent with x = 150...")
//NewMouseMoveEvent mouseEvent2 { 150, 250 };
//dispatcher.Post(mouseEvent2);
//
//// --- Demonstrate dynamic unsubscription ---
//ZN_CORE_INFO("\nDisconnecting free function handler and posting KeyEvent...")
//connFreeFunc->disconnect();
//dispatcher.Post(keyEvent1);
