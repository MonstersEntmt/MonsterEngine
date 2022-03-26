defines({ "SPDLOG_COMPILED_LIB", "FMT_CONSTEVAL=" })

sysincludedirs({ CurrentModule.ModulePath .. "/spdlog/include/" })

files({
	CurrentModule.ModulePath .. "/spdlog/include/**",
	CurrentModule.ModulePath .. "/spdlog/src/**"
})
removefiles({ ".DS_Store" })