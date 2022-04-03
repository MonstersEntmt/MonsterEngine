undefines({ "GLFW_DLL" })
defines({ "_GLFW_BUILD_DLL" })

files({
	CurrentModule.ModulePath .. "/Public/**",
	CurrentModule.ModulePath .. "/Private/**"
})

removefiles({ ".DS_Store" })