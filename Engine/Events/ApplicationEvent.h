#pragma once
#include <cstdint>

namespace zn
{
	struct WindowClosedEvent{};
	
	struct WindowResizedEvent
	{
		int Width;
		int Height;
	};

	struct CursorMovedEvent
	{
		int PosX;
		int PosY;
	};

	struct ScrollChangedEvent
	{
		int OffsetX;
		int OffsetY;
	};
}
