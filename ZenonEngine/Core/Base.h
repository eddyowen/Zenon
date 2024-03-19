#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <sstream>
#include <fstream>

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
}
