files({
	CurrentModule.ModulePath .. CurrentModule.Json.PublicDir .. "/**",
	CurrentModule.ModulePath .. CurrentModule.Json.PrivateDir .. "/**"
})
removefiles({ ".DS_Store" })

filter("system:macosx")
	links({
		"Foundation.framework",
		"QuartzCore.framework",
		"Metal.framework"
	})

filter({})