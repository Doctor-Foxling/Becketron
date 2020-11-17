#pragma once

#ifdef BT_PLATFORM_WINDOWS
	#ifdef BTRON_BUILD_DLL
		#define BTRON_API __declspec(dllexport)
	#else
		#define BTRON_API __declspec(dllimport)
	#endif
#else
	#error Becketron only supports Windows
#endif