#pragma once

#include <functional>
#include <format>

#ifdef ZN_DEBUG
	#if defined(ZN_WINDOWS_PLATFORM)
		#define ZN_DEBUGBREAK() __debugbreak()
	#endif
	#define ZN_ENABLE_ASSERTS
#else
	#define ZN_DEBUGBREAK()
#endif

#define ZN_STRINGIFY_MACRO(x) #x

namespace zn
{
	template<typename T>
	using UniquePtr = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr UniquePtr<T> CreateUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using SharedPtr = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr SharedPtr<T> CreateShared(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WeakPtr = std::weak_ptr<T>;

	template<typename T>
	using EnableSharedFromThis = std::enable_shared_from_this<T>;
}
