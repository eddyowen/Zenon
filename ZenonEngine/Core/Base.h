#pragma once

#include <string>
#include <sstream>

#define BIT(x) (1 << x)

#define ZN_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
