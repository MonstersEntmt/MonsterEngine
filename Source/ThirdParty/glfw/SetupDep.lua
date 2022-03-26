defines({ "GLFW_INCLUDE_NONE" })

filter("system:linux")
	links({ "dl" })
	linkoptions({ "-pthread" })

filter("system:macosx")
	links({
		"CoreGraphics.framework",
		"IOKit.framework",
		"AppKit.framework"
	})

filter({})

sysincludedirs({ CurrentDependencyModule.ModulePath .. "/glfw/include/" })