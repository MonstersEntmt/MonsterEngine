#pragma once

#include <BuildMacros.h>

#if BUILD_IS_TOOLSET_MSVC
	#define ME_MODULE_EXPORT __declspec(dllexport)
	#define ME_MODULE_IMPORT __declspec(dllimport)
#elif BUILD_IS_TOOLSET_CLANG || BUILD_IS_TOOLSET_GCC
	#if BUILD_IS_SYSTEM_WINDOWS
		#define ME_MODULE_EXPORT __attribute__((dllexport))
		#define ME_MODULE_IMPORT __attribute__((dllimport))
	#else
		#define ME_MODULE_EXPORT __attribute__((visibility("default")))
		#define ME_MODULE_IMPORT __attribute__((visibility("default")))
	#endif
#else
	#error Toolset not supported!
#endif

#define MonsterEngine_Core_API ME_MODULE_IMPORT
#define MonsterEngine_WindowManager_API ME_MODULE_IMPORT
#define MonsterEngine_ModuleManager_API ME_MODULE_IMPORT

#define MonsterEditor_Core_API ME_MODULE_EXPORT
