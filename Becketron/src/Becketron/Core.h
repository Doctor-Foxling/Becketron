#pragma once

#include <memory>

#ifdef BT_PLATFORM_WINDOWS

#if BT_DYNAMIC_LINK
	#ifdef BTRON_BUILD_DLL
		#define BTRON_API __declspec(dllexport)
	#else
		#define BTRON_API __declspec(dllimport)
	#endif
#else
	// If not making the engine as a dll then define BTRON_API as nothing
	#define BTRON_API
#endif

#else
	#error Becketron only supports Windows
#endif

#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERTS
#endif

#ifdef BT_ENABLE_ASSERTS
	#define BT_ASSERT(x, ...) { if(!(x)) {BT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define BT_CORE_ASSERT(x, ...) { if(!(x)) {BT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define BT_ASSERT(x, ...)
	#define BT_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Becketron{

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}