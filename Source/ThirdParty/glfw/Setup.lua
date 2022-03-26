files({
	CurrentModule.ModulePath .. "/glfw/include/**",
	CurrentModule.ModulePath .. "/glfw/src/context.c",
	CurrentModule.ModulePath .. "/glfw/src/init.c",
	CurrentModule.ModulePath .. "/glfw/src/input.c",
	CurrentModule.ModulePath .. "/glfw/src/internal.h",
	CurrentModule.ModulePath .. "/glfw/src/mappings.h",
	CurrentModule.ModulePath .. "/glfw/src/monitor.c",
	CurrentModule.ModulePath .. "/glfw/src/null_*",
	CurrentModule.ModulePath .. "/glfw/src/platform.h",
	CurrentModule.ModulePath .. "/glfw/src/platform.c",
	CurrentModule.ModulePath .. "/glfw/src/vulkan.c",
	CurrentModule.ModulePath .. "/glfw/src/window.c",
	CurrentModule.ModulePath .. "/glfw/src/egl_*",
	CurrentModule.ModulePath .. "/glfw/src/osmesa_*"
})

filter("system:windows")
	files({
		CurrentModule.ModulePath .. "/glfw/src/win32_*",
		CurrentModule.ModulePath .. "/glfw/src/wgl_*"
	})

	defines({ "_GLFW_WIN32" })

filter("system:linux")
	files({
		CurrentModule.ModulePath .. "/glfw/src/linux_*",
		CurrentModule.ModulePath .. "/glfw/src/posix_*",
		CurrentModule.ModulePath .. "/glfw/src/xkb_*",
		CurrentModule.ModulePath .. "/glfw/src/glx_*"
	})

if _OPTIONS["glfw-use-wayland"] then
	files({ CurrentModule.ModulePath .. "/glfw/src/wl_*" })
	defines({ "_GLFW_WAYLAND" })
else
	files({ CurrentModule.ModulePath .. "/glfw/src/x11_*" })
	defines({ "_GLFW_X11" })
end

filter("system:macosx")
	files({
		CurrentModule.ModulePath .. "/glfw/src/cocoa_*",
		CurrentModule.ModulePath .. "/glfw/src/nsgl_*",
		CurrentModule.ModulePath .. "/glfw/src/posix_*"
	})
	removefiles({
		CurrentModule.ModulePath .. "/glfw/src/posix_time.h",
		CurrentModule.ModulePath .. "/glfw/src/posix_time.c"
	})

	defines({ "_GLFW_COCOA" })

filter({})