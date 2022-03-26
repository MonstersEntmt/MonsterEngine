#include <cstdlib>

int main(int argc, char** argv)
{
	// TODO: Implement entrypoint
}

#if BUILD_IS_SYSTEM_WINDOWS

#undef APIENTRY
#include <Windows.h>

int WinMain([[maybe_unused]] _In_ HINSTANCE hInstance, [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance, [[maybe_unused]] _In_ LPSTR lpCmdLine, [[maybe_unused]] _In_ int nShowCmd)
{
	return main(__argc, __argv);
}

#endif