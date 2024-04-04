#pragma once

#include <source_location>

#include "Core/Base.h"
#include "Core/Log.h"

#ifdef ZN_ENABLE_ASSERTS
	#define ZN_CORE_ASSERT(check) { if (!(check))\
		{\
			const std::source_location& location = std::source_location::current();\
			ZN_CORE_ERROR("Asserttion Failed ({0}) at {1}:{2}", ZN_STRINGIFY_MACRO(check), location.file_name(), location.line());\
			ZN_DEBUGBREAK();\
		}}
#else
	#define ZN_CORE_ASSERT(check)
#endif