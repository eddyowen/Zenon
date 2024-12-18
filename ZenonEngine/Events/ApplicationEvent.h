#pragma once

#include "Event.h"

namespace zn
{
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;

		ZN_DEFINE_EVENT_TYPE(WindowClosed)
	};

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(int width, int height)
			: m_width(width), m_height(height) {}

		ZN_DEFINE_EVENT_TYPE(WindowResized)
	
	private:
		int m_width;
		int m_height;
	};
}
