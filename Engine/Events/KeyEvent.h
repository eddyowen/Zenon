#pragma once

#include "Events/Event.h"
#include "Core/KeyCodes.h"

namespace zn
{
	struct KeyPressedEvent
	{
		KeyCode KeyCode;
		uint16_t RepeatCount;
	};

	struct KeyReleasedEvent{};
}