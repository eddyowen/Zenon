#pragma once

#include "Core/Base.h"
#include "Input/KeyCodes.h"

namespace zn
{
	struct KeyPressedEvent
	{
		KeyCode KeyCode;
		u16 RepeatCount;
	};

	struct KeyReleasedEvent{};
}