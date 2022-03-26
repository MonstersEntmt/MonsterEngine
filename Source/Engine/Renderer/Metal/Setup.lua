files({
	CurrentModule.ModulePath .. CurrentModule.Json.PublicDir .. "/**",
	CurrentModule.ModulePath .. CurrentModule.Json.PrivateDir .. "/**"
})
removefiles({ ".DS_Store" })

buildoptions({ "-x objective-c++" })

--filter("files:**.cpp")
--compileas("Objective-C++")

filter("system:macosx")
	links({
		"Foundation.framework",
		"QuartzCore.framework",
		"Metal.framework"
	})

filter({})