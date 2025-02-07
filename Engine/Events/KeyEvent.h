#pragma once

#include "Events/Event.h"
#include "Input/KeyCodes.h"

namespace zn
{
	struct KeyPressedEvent
	{
		KeyCode KeyCode;
		uint16_t RepeatCount;
	};

	struct KeyReleasedEvent{};
}