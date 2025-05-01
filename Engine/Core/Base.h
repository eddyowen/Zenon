#pragma once

#include <cstdint>       
#include <vector>        
#include <string>        
#include <string_view>   
#include <map>           
#include <unordered_map> 
#include <set>           
#include <unordered_set> 
#include <memory>        
#include <utility>       
#include <optional>      
#include <functional>
#include <type_traits>

#ifdef ZN_DEBUG
	#if defined(ZN_WINDOWS_PLATFORM)
		#define ZN_DEBUGBREAK() __debugbreak()
	#endif
	#define ZN_ENABLE_ASSERTS
#else
	#define ZN_DEBUGBREAK()
#endif

#define ZN_STRINGIFY_MACRO(x) #x

// You might place these aliases within a specific namespace
// namespace core {
// or
// namespace common {
// or leave them in the global scope if preferred (less recommended for larger projects)

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
// Size and Pointer Difference Types
//-----------------------------------------------------------------------------
using sizet   = std::size_t;
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
// Standard Container Types
//-----------------------------------------------------------------------------
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
