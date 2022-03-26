#include "Module.h"

#if BUILD_IS_SYSTEM_WINDOWS
#include <Windows.h>
#elif BUILD_IS_SYSTEM_MACOSX || BUILD_IS_SYSTEM_LINUX
#include <dlfcn.h>
#endif

IMonsterModule::IMonsterModule(Version version)
    : m_Version(version) {}

void* IMonsterModule::getProcAddress(const std::string& name)
{
#if BUILD_IS_SYSTEM_WINDOWS
	return static_cast<void*>(GetProcAddress(static_cast<HMODULE>(m_NativeHandle), name.c_str()));
#elif BUILD_IS_SYSTEM_MACOSX || BUILD_IS_SYSTEM_LINUX
	return dlsym(m_NativeHandle, name.c_str());
#endif
}