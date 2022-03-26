#pragma once

#define BUILD_CONFIG_UNKNOWN 0
#define BUILD_CONFIG_DEBUG   1
#define BUILD_CONFIG_RELEASE 2
#define BUILD_CONFIG_DIST    3

#define BUILD_SYSTEM_UNKNOWN 0
#define BUILD_SYSTEM_WINDOWS 1
#define BUILD_SYSTEM_MACOSX  2
#define BUILD_SYSTEM_LINUX   3

#define BUILD_TOOLSET_UNKNOWN 0
#define BUILD_TOOLSET_MSVC    1
#define BUILD_TOOLSET_CLANG   2
#define BUILD_TOOLSET_GCC     3

#define BUILD_PLATFORM_UNKNOWN 0
#define BUILD_PLATFORM_AMD64   1

#define BUILD_IS_CONFIG_DEBUG (BUILD_CONFIG == BUILD_CONFIG_DEBUG)
#define BUILD_IS_CONFIG_DIST  ((BUILD_CONFIG == BUILD_CONFIG_RELEASE) || (BUILD_CONFIG == BUILD_CONFIG_DIST))

#define BUILD_IS_SYSTEM_WINDOWS (BUILD_SYSTEM == BUILD_SYSTEM_WINDOWS)
#define BUILD_IS_SYSTEM_MACOSX  (BUILD_SYSTEM == BUILD_SYSTEM_MACOSX)
#define BUILD_IS_SYSTEM_LINUX   (BUILD_SYSTEM == BUILD_SYSTEM_LINUX)
#define BUILD_IS_SYSTEM_UNIX    (BUILD_IS_SYSTEM_MACOSX || BUILD_IS_SYSTEM_LINUX)

#define BUILD_IS_TOOLSET_MSVC  (BUILD_TOOLSET == BUILD_TOOLSET_MSVC)
#define BUILD_IS_TOOLSET_CLANG (BUILD_TOOLSET == BUILD_TOOLSET_CLANG)
#define BUILD_IS_TOOLSET_GCC   (BUILD_TOOLSET == BUILD_TOOLSET_GCC)

#define BUILD_IS_PLATFORM_AMD64 (BUILD_PLATFORM == BUILD_PLATFORM_AMD64)

namespace MonsterEngine
{
#if BUILD_IS_CONFIG_DEBUG
	static constexpr bool s_IsConfigDebug = true;
	static constexpr bool s_IsConfigDist  = false;
#elif BUILD_IS_CONFIG_RELEASE
	static constexpr bool s_IsConfigDebug   = true;
	static constexpr bool s_IsConfigDist    = true;
#elif BUILD_IS_CONFIG_DIST
	static constexpr bool s_IsConfigDebug   = false;
	static constexpr bool s_IsConfigDist    = true;
#else
	static constexpr bool s_IsConfigDebug   = false;
	static constexpr bool s_IsConfigDist    = false;
#endif

#if BUILD_IS_SYSTEM_WINDOWS
	static constexpr bool s_IsSystemWindows = true;
	static constexpr bool s_IsSystemMacosx  = false;
	static constexpr bool s_IsSystemLinux   = false;
	static constexpr bool s_IsSystemUnix    = false;
#elif BUILD_IS_SYSTEM_MACOSX
	static constexpr bool s_IsSystemWindows = false;
	static constexpr bool s_IsSystemMacosx  = true;
	static constexpr bool s_IsSystemLinux   = false;
	static constexpr bool s_IsSystemUnix    = true;
#elif BUILD_IS_SYSTEM_LINUX
	static constexpr bool s_IsSystemWindows = false;
	static constexpr bool s_IsSystemMacosx  = false;
	static constexpr bool s_IsSystemLinux   = true;
	static constexpr bool s_IsSystemUnix    = true;
#else
	static constexpr bool s_IsSystemWindows = false;
	static constexpr bool s_IsSystemMacosx  = false;
	static constexpr bool s_IsSystemLinux   = false;
	static constexpr bool s_IsSystemUnix    = false;
#endif

#if BUILD_IS_TOOLSET_MSVC
	static constexpr bool s_IsToolsetMSVC  = true;
	static constexpr bool s_IsToolsetClang = false;
	static constexpr bool s_IsToolsetGCC   = false;
#elif BUILD_IS_TOOLSET_CLANG
	static constexpr bool s_IsToolsetMSVC   = false;
	static constexpr bool s_IsToolsetClang  = true;
	static constexpr bool s_IsToolsetGCC    = false;
#elif BUILD_IS_TOOLSET_GCC
	static constexpr bool s_IsToolsetMSVC   = false;
	static constexpr bool s_IsToolsetClang  = false;
	static constexpr bool s_IsToolsetGCC    = true;
#else
	static constexpr bool s_IsToolsetMSVC   = false;
	static constexpr bool s_IsToolsetClang  = false;
	static constexpr bool s_IsToolsetGCC    = false;
#endif

#if BUILD_IS_PLATFORM_AMD64
	static constexpr bool s_IsToolsetAMD64 = true;
#else
	static constexpr bool s_IsToolsetAMD64  = false;
#endif
} // namespace MonsterEngine