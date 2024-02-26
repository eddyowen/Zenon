#pragma once

#include "Core/Base.h"

#include <queue>
#include <optional>

namespace zn
{
	enum class EventType
	{
		None = 0,
		WindowClosed,
		KeyPressed, KeyReleased
	};

#define ZN_DEFINE_EVENT_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

	class Event
	{
	public:
		~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;

		virtual std::string ToString() const { return GetName(); };

	protected:
		EventType m_Type = EventType::None;
	};

	//class EventBuffer
	//{
	//public:
	//	using EventQueue = std::queue<zn::Event>;
	//
	//	EventBuffer() = default;
	//	~EventBuffer() = default;
	//
	//	void Push(const zn::Event& event)
	//	{
	//		buffer.push(event);
	//	}
	//
	//	std::optional<zn::Event> Pop()
	//	{
	//		if (buffer.empty())
	//		{
	//			return std::nullopt;
	//		}
	//
	//		Event event = buffer.front();
	//		buffer.pop();
	//
	//		return event;
	//	}
	//
	//	bool IsEmpty() const { return buffer.empty(); }
	//
	//private:
	//	EventQueue buffer;
	//};
}
