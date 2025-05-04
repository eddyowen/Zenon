#pragma once

#include "Core/Base.hpp"
#include "Input/KeyCodes.hpp"

namespace zn
{
	struct KeyPressedEvent
	{
		KeyCode KeyCode;
		u16 RepeatCount;
	};

	struct KeyReleasedEvent{};
}