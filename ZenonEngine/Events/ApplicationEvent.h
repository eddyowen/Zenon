#pragma once

#include "Event.h"

namespace zn
{
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;

		ZN_DEFINE_EVENT_TYPE(WindowClosed);
	};
}
