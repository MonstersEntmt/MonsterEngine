newoption({
	trigger     = "vulkan-sdK",
	description = "Overrides VULKAN_SDK environment variable",
	value       = "path",
	default     = os.getenv("VULKAN_SDK")
})

files({
	CurrentModule.ModulePath .. CurrentModule.Json.PublicDir .. "/**",
	CurrentModule.ModulePath .. CurrentModule.Json.PrivateDir .. "/**"
})
removefiles({ ".DS_Store" })

local sdkPath     = path.translate(_OPTIONS["vulkan-sdk"], "/")
local includePath = ""
local libPath     = ""
if MonsterEngine.PremakeUtils.Host == "windows" then
	includePath = "/Include/"
	libPath     = "/Lib/"
else
	includePath = "/include/"
	libPath     = "/lib/"
end

local requiredSDKLibs = {
	All = {},
	Debug = {},
	Dist = {}
}

if MonsterEngine.PremakeUtils.Host == "windows" then
	table.insert(requiredSDKLibs.All, "vulkan-1")
	table.insert(requiredSDKLibs.Dist, "shaderc_combined")
	table.insert(requiredSDKLibs.Debug, "shaderc_combinedd")
else
	table.insert(requiredSDKLibs.All, "vulkan")
	table.insert(requiredSDKLibs.All, "shaderc_combined")
end

sysincludedirs({ sdkPath .. includePath })

libdirs({ sdkPath .. libPath })
links(requiredSDKLibs.All)
filter("configurations:Debug")
	links(requiredSDKLibs.Debug)

filter("configurations:not Debug")
	links(requiredSDKLibs.Dist)

filter({})