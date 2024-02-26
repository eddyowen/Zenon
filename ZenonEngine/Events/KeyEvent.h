#pragma once

#include "Events/Event.h"
#include "Core/KeyCodes.h"

namespace zn
{
	class KeyPressedEvent : public Event 
	{
	public:
		KeyPressedEvent(KeyCode keyCode, uint16_t repeatCount) 
			: m_keyCode(keyCode), m_repeatCount(repeatCount) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss{};
			ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		};

		ZN_DEFINE_EVENT_TYPE(KeyPressed)

	private:
		KeyCode m_keyCode;
		uint16_t m_repeatCount;
	};

	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent() = default;

		ZN_DEFINE_EVENT_TYPE(KeyPressed)
	};
}