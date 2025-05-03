#pragma once

#include "Core/Base.h"
#include "Core/Log.h"

#include <source_location>

#ifdef ZN_ENABLE_ASSERTS
	#define ZN_CORE_ASSERT_IMPL_NOMSG(condition) \
		do { \
			if (!(condition)) \
			{ \
				const std::source_location& location = std::source_location::current(); \
				ZN_CORE_ERROR("Assertion Failed ({}). Location: {}:{}", #condition, location.file_name(), location.line()); \
				ZN_DEBUGBREAK(); \
			} \
		} while(0)

	#define ZN_CORE_ASSERT_IMPL_MSG(condition, ...) \
	    do { \
	        if (!(condition)) { \
	            const auto location = std::source_location::current(); \
	            ZN_CORE_ERROR("Assertion Failed ({}). {}. Location: {}:{}", #condition, __VA_ARGS__, location.file_name(), location.line()); \
				ZN_DEBUGBREAK(); \
	        } \
	    } while(0)

	#define ZN_GET_NTH_ARG(_1, _2, N, ...) N

	#define ZN_ASSERT(...) ZN_EXPAND_MACRO(ZN_GET_NTH_ARG(__VA_ARGS__, ZN_CORE_ASSERT_IMPL_MSG, ZN_CORE_ASSERT_IMPL_NOMSG)(__VA_ARGS__))
#else
	#define ZN_ASSERT(...)
#endif