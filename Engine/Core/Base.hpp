#pragma once

#include <array>
#include <cstdint>       
#include <functional>
#include <map>           
#include <memory>        
#include <optional>      
#include <vector>        
#include <set>           
#include <string>        
#include <string_view>   
#include <type_traits>
#include <unordered_map> 
#include <unordered_set> 
#include <utility>       

#ifdef ZN_DEBUG
	#if defined(ZN_WINDOWS_PLATFORM)
		#define ZN_DEBUGBREAK() __debugbreak()
	#endif
	#define ZN_ENABLE_ASSERTS
#else
	#define ZN_DEBUGBREAK()
#endif

#define ZN_EXPAND_MACRO(x) x

#define ZN_STRINGIFY_MACRO(x) #x

namespace  zn
{
	//-----------------------------------------------------------------------------
	// Integer Types
	//-----------------------------------------------------------------------------
	using i8  = std::int8_t;
	using u8  = std::uint8_t;
	using i16 = std::int16_t;
	using u16 = std::uint16_t;
	using i32 = std::int32_t;
	using u32 = std::uint32_t;
	using i64 = std::int64_t;
	using u64 = std::uint64_t;

	//-----------------------------------------------------------------------------
	// Boolean Types
	//-----------------------------------------------------------------------------
	using b8 = bool; // Less common to alias bool

	//-----------------------------------------------------------------------------
	// Character Types
	//-----------------------------------------------------------------------------
	using c8 = char;         // Usually for ASCII / UTF-8 code units
	using c16 = char16_t;    // For UTF-16 code units
	using c32 = char32_t;    // For UTF-32 code units / Unicode code points
	using wc = wchar_t;      // Wide character (platform-dependent size)

	//-----------------------------------------------------------------------------
	// Size and Pointer Difference Types
	//-----------------------------------------------------------------------------
	using uSize   = std::size_t;
	using diff = std::ptrdiff_t;

	//-----------------------------------------------------------------------------
	// Floating Point Types (Less commonly aliased, but can be useful)
	//-----------------------------------------------------------------------------
	using f32 = float;
	using f64 = double;
	// using f128 = long double; // Optional, long double precision varies

	//-----------------------------------------------------------------------------
	// String Types
	//-----------------------------------------------------------------------------
	using String     = std::string;
	using StringView = std::string_view;

	//-----------------------------------------------------------------------------
	// Memory Types
	//-----------------------------------------------------------------------------
	using Byte = std::byte;

	//-----------------------------------------------------------------------------
	// Numeric Limits
	//-----------------------------------------------------------------------------
	constexpr u32 U32_MAX = std::numeric_limits<u32>::max();
	constexpr i32 I32_MAX = std::numeric_limits<i32>::max();
	constexpr i32 I32_MIN = std::numeric_limits<i32>::min();

	constexpr f32 F32_EPSILON = std::numeric_limits<f32>::epsilon();
	constexpr f64 F64_EPSILON = std::numeric_limits<f64>::epsilon();

	//-----------------------------------------------------------------------------
	// Standard Container Types
	//-----------------------------------------------------------------------------
	template<typename T, uSize N>
	using Array = std::array<T, N>;

	template<typename T>
	using Vector = std::vector<T>;

	template<typename K, typename V>
	using Map = std::map<K, V>;

	template<typename K, typename V>
	using UMap = std::unordered_map<K, V>;

	template<typename T>
	using Set = std::set<T>;

	template<typename T>
	using USet = std::unordered_set<T>;

	//-----------------------------------------------------------------------------
	// Utility Types
	//-----------------------------------------------------------------------------
	template<typename T1, typename T2>
	using Pair = std::pair<T1, T2>;

	template<typename T>
	using Opt = std::optional<T>;

	template<typename T>
	using CRefWrapper = std::reference_wrapper<const T>;

	template<typename T>
	using RefWrapper = std::reference_wrapper<T>;

	//-----------------------------------------------------------------------------
	// Smart Pointers
	//-----------------------------------------------------------------------------
	template<typename Signature>
	using Func = std::function<Signature>;

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

	//-----------------------------------------------------------------------------
	// Concepts
	//-----------------------------------------------------------------------------
	template<typename T>
	concept Moveable = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

	template<typename T, typename... TArgs>
	concept ConstructibleWithArgs = std::is_constructible_v<T, TArgs...>;

	template<typename T, typename... TArgs>
	concept CallableWithArgs = std::is_invocable_v<T, TArgs...>;

	template<typename T, typename... TArgs, typename TReturn>
	concept ReturnsType = std::same_as<std::invoke_result_t<T, TArgs...>, TReturn>;
	
	template<typename T, typename... TArgs, typename... TReturn>
	concept MatchingSignature = CallableWithArgs<T, TArgs...> && ReturnsType<T, TReturn>;

	
}

