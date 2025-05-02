#pragma once

#include "Core/Base.h"

namespace zn
{
	struct WindowClosedEvent{};
	
	struct WindowResizedEvent
	{
		u32 Width;
		u32 Height;
	};

	struct CursorMovedEvent
	{
		f64 PosX;
		f64 PosY;
	};

	struct ScrollChangedEvent
	{
		f64 OffsetX;
		f64 OffsetY;
	};
}
