#pragma once
#include <cstdint>

namespace zn
{
	struct WindowClosedEvent{};
	
	struct WindowResizedEvent
	{
		uint32_t Width;
		uint32_t Height;
	};

	struct CursorMovedEvent
	{
		double PosX;
		double PosY;
	};

	struct ScrollChangedEvent
	{
		double OffsetX;
		double OffsetY;
	};
}
