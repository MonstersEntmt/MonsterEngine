require("Actions/Clean")

newoption({
	trigger     = "target-arch",
	value       = "architectures",
	default     = "host",
	description = "Comma separated list of architectures to build (host == only host architecture)"
})

local function LoadFileTemplates()
	local templates = {}

	local matches = os.matchfiles("Templates/**")
	for _, match in ipairs(matches) do
		verbosef("Template '%s'", match)
		templates[match] = io.readfile(match)
	end

	return templates
end

local MonsterEngine = {
	IsEngine      = true,
	Directory     = "",
	FileTemplates = LoadFileTemplates(),
	ModuleManager = {
		ProjectDirs          = {},
		LoadedEnginePackages = {},
		LoadedPackages       = {},
		PackageHandler       = {
			ModuleHandler = {}
		}
	},
	PremakeUtils = {
		Host        = os.host(),
		Target      = os.target(),
		Arch        = nil,
		TargetArchs = nil
	}
}
local ModuleManager  = MonsterEngine.ModuleManager
local PackageHandler = ModuleManager.PackageHandler
local ModuleHandler  = PackageHandler.ModuleHandler
local PremakeUtils   = MonsterEngine.PremakeUtils

function MonsterEngine:SetAsProject()
	verbosef("MonsterEngine:SetAsProject()")
	self.IsEngine = false
end

function MonsterEngine:SetEngineDirectory(directory)
	verbosef("MonsterEngine:SetEngineDirectory(directory)")
	self.Directory = directory
end

function MonsterEngine:LoadPackages()
	verbosef("MonsterEngine:LoadPackages()")
	local enginePackageFiles = os.matchfiles(self.Directory .. "Source/**/Package.json")
	for _, enginePackageFile in ipairs(enginePackageFiles) do
		local package = self.ModuleManager.PackageHandler:LoadPackage(enginePackageFile)
		if package then
			package.SearchDirectory = self.Directory .. "Source/"
			table.insert(self.ModuleManager.LoadedEnginePackages, package)
		end
	end
	for _, projectDir in pairs(self.ModuleManager.ProjectDirs) do
		local packageFiles = os.matchFiles(projectDir .. "Source/**/Package.json")
		for _, packageFile in ipairs(packageFiles) do
			local package = self.ModuleManager.PackageHandler:LoadPackage(packageFile)
			if package then
				package.SearchDirectory = projectDir .. "Source/"
				table.insert(self.ModuleMangaer.LoadedPackages, package)
			end
		end
	end
end

function ModuleManager:AddProjectDirectory(directory)
	verbosef("ModuleManager:AddProjectDirectory(directory)")
	table.insert(self.ProjectDirs, directory)
end

function ModuleManager:AddPackage(package)
	verbosef("ModuleManager:AddPackage(package)")
	if package then
		table.insert(self.LoadedPackages, package)
	end
end

function ModuleManager:GetModule(moduleID)
	verbosef("ModuleManager:GetModule(moduleID)")
	for _, package in ipairs(self.LoadedEnginePackages) do
		if moduleID:find(package.Json.Name, 1, #package.Json.Name) then
			for _, module in ipairs(package.Modules) do
				if module:GetModuleID() == moduleID then
					return module
				end
			end
		end
	end
	for _, package in ipairs(self.LoadedPackages) do
		if moduleID:find(package.Json.Name, 1, #package.Json.Name) then
			for _, module in ipairs(package.Modules) do
				if module:GetModuleID() == moduleID then
					return module
				end
			end
		end
	end
	return nil
end

function PackageHandler:New(nameOrJson, packagePath)
	verbosef("PackageHandler:New(nameOrJson, packagePath)")
	local package = {
		Json = {
			Name            = "",
			SetupScriptPath = ""
		},
		Modules         = {},
		PackagePath     = "",
		SearchDirectory = ""
	}
	setmetatable(package, self)
	self.__index = self

	if type(nameOrJson) == "table" then
		package.Json.Name            = nameOrJson.Name or package.Json.Name
		package.Json.SetupScriptPath = nameOrJson.SetupScriptPath or package.Json.SetupScriptPath
	elseif type(nameOrJson) == "string" then
		package.Json.Name = nameOrJson
	end
	package.PackagePath = packagePath

	return package
end

function PackageHandler:LoadPackage(packageFile)
	verbosef("PackageHandler:LoadPackage(packageFile)")
	local jsonText  = io.readfile(packageFile)
	local json, err = json.decode(jsonText)
	if not json then
		print("'" .. packageFile .. "' Failed to load properly: '" .. err .. "'")
		return nil
	end

	local packagePath = path.getdirectory(packageFile) .. "/"
	local package = self:New(json, packagePath)
	package:LoadModules()
	return package
end

function PackageHandler:SavePackage()
	verbosef("PackageHandler:SavePackage()")
	io.writefile(self.PackagePath .. "Package.json", json.encode_pretty(self.Json))
end

function PackageHandler:AddModule(module)
	if module then
		table.insert(self.Modules, module)
	end
end

function PackageHandler:LoadModules()
	verbosef("PackageHandler:LoadModules()")
	local moduleFiles = os.matchfiles(self.PackagePath .. "**/Module.json")
	for _, moduleFile in ipairs(moduleFiles) do
		local module = ModuleHandler:LoadModule(moduleFile, self)
		if module then
			table.insert(self.Modules, module)
		end
	end
end

function ModuleHandler:New(package, nameOrJson, modulePath, moduleType, withPCH)
	verbosef("ModuleHandler:New(package, nameOrJson, modulePath, moduleType, withPCH)")
	local module = {
		Json = {
			Name                = "",
			Type                = "Module",
			PublicDir           = "Public/",
			PrivateDir          = "Private/",
			Version             = { 0, 0, 0 },
			Dependencies        = {},
			PCH                 = nil,
			PreBuildScriptPath  = "",
			PostBuildScriptPath = "",
			SetupScriptPath     = "",
			SetupDepScriptPath  = ""
		},
		Package    = package,
		ModulePath = ""
	}
	setmetatable(module, self)
	self.__index = self

	if type(nameOrJson) == "table" then
		module.Json.Name                = nameOrJson.Name                or module.Json.Name
		module.Json.Type                = nameOrJson.Type                or module.Json.Type
		module.Json.PublicDir           = nameOrJson.PublicDir           or module.Json.PublicDir
		module.Json.PrivateDir          = nameOrJson.PrivateDir          or module.Json.PrivateDir
		module.Json.Version             = nameOrJson.Version             or module.Json.Version
		module.Json.Dependencies        = nameOrJson.Dependencies        or module.Json.Dependencies
		module.Json.PCH                 = nameOrJson.PCH                 or module.Json.PCH
		module.Json.PreBuildScriptPath  = nameOrJson.PreBuildScriptPath  or module.Json.PreBuildScriptPath
		module.Json.PostBuildScriptPath = nameOrJson.PostBuildScriptPath or module.Json.PostBuildScriptPath
		module.Json.SetupScriptPath     = nameOrJson.SetupScriptPath     or module.Json.SetupScriptPath
		module.Json.SetupDepScriptPath  = nameOrJson.SetupDepScriptPath  or module.Json.SetupDepScriptPath
	elseif type(nameOrJson) == "string" then
		module.Json.Name = nameOrJson
		module.Json.Type = moduleType or module.Json.Type

		if withPCH then
			module.Json.PCH = {
				Header = "PCH.h",
				Source = "PCH.cpp"
			}
		end
	end
	module.ModulePath = modulePath

	return module
end

function ModuleHandler:LoadModule(moduleFile, package)
	verbosef("ModuleHandler:LoadModule(moduleFile, package)")
	local jsonText  = io.readfile(moduleFile)
	local json, err = json.decode(jsonText)
	if not json then
		print("'" .. moduleFile .. "' Failed to load properly: '" .. err .. "'")
		return nil
	end
	
	local modulePath = path.getdirectory(moduleFile) .. "/"
	return self:New(package, json, modulePath)
end

function ModuleHandler:SaveModule()
	verbosef("ModuleHandler:SaveModule()")
	io.writefile(self.ModulePath .. "Module.json", json.encode_pretty(self.Json))
end

function ModuleHandler:GetModuleID()
	verbosef("ModuleHandler:GetModuleID()")
	return self.Package.Json.Name .. "." .. self.Json.Name
end

function ModuleHandler:GetModuleIDNamespace()
	verbosef("ModuleHandler:GetModuleIDNamespace()")
	return self:GetModuleID():gsub("%.", "::")
end

function ModuleHandler:GetModuleIDDefine()
	verbosef("ModuleHandler:GetModuleIDDefine()")
	return self:GetModuleID():gsub("%.", "_")
end

function ModuleHandler:GetModuleIDPath()
	verbosef("ModuleHandler:GetModuleIDPath()")
	return self:GetModuleID():gsub("%.", "/") .. "/"
end

function ModuleHandler:GetRelativeDir()
	verbosef("ModuleHandler:GetRelativeDir()")
	return path.translate(path.getrelative(self.Package.SearchDirectory, self.ModulePath), "/")
end

function PremakeUtils:SetupBuildCommands()
	verbosef("PremakeUtils:SetupBuildCommands()")
	newaction({
		trigger     = "prebuild",
		shortname   = "Pre Build Script",
		description = "Call Pre Build Script"
	})

	newaction({
		trigger     = "postbuild",
		shortname   = "Post Build Script",
		description = "Call Post Build Script"
	})

	newoption({
		trigger     = "module",
		value       = "Module",
		description = "The module to call prebuild/postbuild on"
	})

	newoption({
		trigger     = "moduleout",
		value       = "path",
		description = "The path to the modules output"
	})
end

function PremakeUtils:HandleBuildCommands()
	verbosef("PremakeUtils:HandleBuildCommands()")
	if _ACTION == "prebuild" or _ACTION == "postbuild" then
		MonsterEngine:LoadPackages()
		local module = ModuleManager:GetModule(_OPTIONS["module"])
		if not module then
			error("Given module is non existent: " .. _OPTIONS["module"])
		end
		
		if _ACTION == "prebuild" then
			if module.Json.PreBuildScriptPath:len() > 0 then
				dofile(module.ModulePath .. module.Json.PreBuildScriptPath)
			end
		elseif _ACTION == "postbuild" then
			if module.Json.Type == "Module" then
				local output     = path.getabsolute(_OPTIONS["moduleout"], _WORKING_DIR)
				local outputDir  = path.getdirectory(output)
				local iniContent = "Version = " .. module.Json.Version[1] .. ", ".. module.Json.Version[2] .. ", " .. module.Json.Version[3] .. "\n\n[Dependencies]\n"
				
				local moduleDeps = {}
				table.insert(moduleDeps, "MonsterEngine.ModuleManager")
				for _, dep in ipairs(module.Json.Dependencies) do
					table.insert(moduleDeps, dep)
				end

				for _, dependency in ipairs(moduleDeps) do
					local dependencyModule = ModuleManager:GetModule(dependency)
					if dependencyModule and (dependencyModule.Json.Type == "Module" or dependencyModule.Json.Type == "ModuleManager") then
						iniContent = iniContent .. dependencyModule:GetModuleID() .. " = " .. dependencyModule.Json.Version[1] .. ", " .. dependencyModule.Json.Version[2] .. ", " .. dependencyModule.Json.Version[3] .. "\n"
					end
				end
				io.writefile(path.replaceextension(output, "ini"), iniContent)
			end

			if module.Json.PostBuildScriptPath:len() > 0 then
				dofile(module.ModulePath .. module.Json.PostBuildScriptPath)
			end
		end
		return true
	end
	return false
end

function PremakeUtils:HandleDependency(dep, dependencies, sysIncludeDirs)
	if dep.Json.SetupDepScriptPath:len() > 0 then
		CurrentDependencyModule = dep
		dofile(dep.ModulePath .. dep.Json.SetupDepScriptPath)
	else
		table.insert(sysIncludeDirs, dep.ModulePath .. dep.Json.PublicDir)
	end
	
	for _, depend in ipairs(dep.Json.Dependencies) do
		local dependencyModule = ModuleManager:GetModule(depend)
		if dependencyModule then
			table.insert(dependencies, depend)
			self:HandleDependency(dependencyModule, dependencies, sysIncludeDirs)
		end
	end
end

function PremakeUtils:SetupWorkspace()
	verbosef("PremakeUtils:SetupWorkspace()")
	configurations({ "Debug", "Release", "Dist" })
	platforms(self.TargetArchs)
	debugdir("%{wks.location}/Temp/")

	for _, v in ipairs(self.TargetArchs) do
		filter("platforms:" .. v)
			defines({ "BUILD_PLATFORM=BUILD_PLATFORM_" .. v:upper() })
			architecture(v)
	end

	filter("configurations:Debug")
		defines({ "BUILD_CONFIG=BUILD_CONFIG_DEBUG" })
		optimize("Off")
		symbols("On")

	filter("configurations:Release")
		defines({ "BUILD_CONFIG=BUILD_CONFIG_RELEASE" })
		optimize("Full")
		symbols("On")

	filter("configurations:Dist")
		defines({ "BUILD_CONFIG=BUILD_CONFIG_DIST" })
		optimize("Full")
		symbols("Off")

	filter("system:windows")
		toolset("msc")
		defines({
			"BUILD_SYSTEM=BUILD_SYSTEM_WINDOWS",
			"NOMINMAX", -- Windows.h disables
			"WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS"
		})

	filter("system:macosx")
		toolset("clang")
		defines({ "BUILD_SYSTEM=BUILD_SYSTEM_MACOSX" })

	filter("system:linux")
		toolset("clang") -- Yes we hate gnu so why use gcc as the default toolset, just deal with it
		defines({ "BUILD_SYSTEM=BUILD_SYSTEM_LINUX" })

	filter("toolset:msc")
		defines({ "BUILD_TOOLSET=BUILD_TOOLSET_MSVC" })

	filter("toolset:clang")
		defines({ "BUILD_TOOLSET=BUILD_TOOLSET_CLANG" })

	filter("toolset:gcc")
		defines({ "BUILD_TOOLSET=BUILD_TOOLSET_GCC" })

	filter({})

	cppdialect("C++20")
	rtti("On")
	exceptionhandling("On")
	flags("MultiProcessorCompile")
end

function PremakeUtils:SetupModule(module)
	verbosef("PremakeUtils:SetupModule(module)")
	self:CreateMissingProjectFiles(module)

	local moduleID = module:GetModuleID()

	local moduleFolder = module:GetRelativeDir()
	local moduleGroup  = moduleFolder
	if moduleGroup:sub(-module.Json.Name:len()) == module.Json.Name then
		moduleGroup = moduleGroup:sub(1, -module.Json.Name:len() - 2)
	end
	CurrentModule = module
	group(moduleGroup)
	project(moduleID)
		-- TODO: Implement projectname(module.Json.Name)
		filename(module.Json.Name)
		location(module.ModulePath)

	local moduleDeps = {}
	for _, dep in ipairs(module.Json.Dependencies) do
		table.insert(moduleDeps, dep)
	end
	if module.Json.Type == "Module" or module.Json.Type == "Executable" then
		table.insert(moduleDeps, "MonsterEngine.ModuleManager")
	end

	local dependencies   = {}
	local sysIncludeDirs = {}
	for _, dependency in ipairs(moduleDeps) do
		local dependencyModule = ModuleManager:GetModule(dependency)
		if dependencyModule then
			table.insert(dependencies, dependency)
			self:HandleDependency(dependencyModule, dependencies, sysIncludeDirs)
		end
	end

		links(dependencies)
		sysincludedirs(sysIncludeDirs)

		includedirs({
			"%{prj.location}/" .. module.Json.PublicDir,
			"%{prj.location}/" .. module.Json.PrivateDir
		})

	if module.Json.Type == "Executable" then
		filter("configurations:Debug")
			kind("ConsoleApp")

		filter("configurations:not Debug")
			kind("WindowedApp")

		filter({})
	elseif module.Json.Type == "Module" or module.Json.Type == "ModuleManager" or module.Json.Type == "SharedLibrary" then
		kind("SharedLib")
	elseif module.Json.Type == "StaticLibrary" then
		kind("StaticLib")
	end

	if module.Json.Type == "Module" or module.Json.Type == "StaticLibrary" or module.Json.Type == "SharedLibrary" then
		targetdir("%{wks.location}/Binaries/%{cfg.system}-%{cfg.platform}-%{cfg.buildcfg}/" .. moduleFolder .. "/")
	else
		targetdir("%{wks.location}/Binaries/%{cfg.system}-%{cfg.platform}-%{cfg.buildcfg}/")
	end
		objdir("%{wks.location}/Binaries/Int-%{cfg.system}-%{cfg.platform}-%{cfg.buildcfg}/" .. moduleFolder .. "/" .. module.Json.Name .. "/")

	if module.Json.Type == "ModuleManager" or module.Json.Type == "StaticLibrary" then
		targetname(module.Json.Name)
	elseif module.Json.Type == "Executable" then
		targetname(module.Package.Json.Name)
	end

	local pch = module.Json.PCH
	if pch then
		local pchHPath = module.ModulePath .. module.PrivateDir .. pch.Header
		if not os.isfile(pchHPath) then
			io.writefile(pchHPath, "#pragma once\n")
		end
		local pchCppPath = module.ModulePath .. module.PrivateDir .. pch.Source
		if not os.isfile(pchCppPath) then
			io.writefile(pchCppPath, "#include \"" .. pch.Header .. "\"")
		end
		pchheader(pch.Header)
		pchsource(pchCppPath)

		forceincludes({ pch.Header })
	end

		filter("system:windows")
	if module.Json.PreBuildScriptPath then
			prebuildcommands({ "\"" .. _PREMAKE_COMMAND .. "\" prebuild \"--file=%{!wks.location}\\premake5.lua\" \"--module=" .. module:GetModuleID() .. "\" \"--moduleout=%{!cfg.buildtarget.abspath}\"" })
	end
			postbuildcommands({ "\"" .. _PREMAKE_COMMAND .. "\" postbuild \"--file=%{!wks.location}\\premake5.lua\" \"--module=" .. module:GetModuleID() .. "\" \"--moduleout=%{!cfg.buildtarget.abspath}\"" })
		filter("system:not windows")
    if module.Json.PreBuildScriptPath then
			prebuildcommands({ "'" .. _PREMAKE_COMMAND .. "' prebuild '--file=%{!wks.location}/premake5.lua' '--module=" .. module:GetModuleID() .. "' '--moduleout=%{!cfg.buildtarget.abspath}'" })
	end
   			postbuildcommands({ "'" .. _PREMAKE_COMMAND .. "' postbuild '--file=%{!wks.location}/premake5.lua' '--module=" .. module:GetModuleID() .. "' '--moduleout=%{!cfg.buildtarget.abspath}'" })
		filter({})

	if module.Json.Type ~= "StaticLibrary" and module.Json.Type ~= "SharedLibrary" then
		forceincludes({ "Defines.h" })
	end

		pic("On")
	if module.Json.SetupScriptPath:len() > 0 then
		dofile(module.ModulePath .. module.Json.SetupScriptPath)
	else
		files({
			"%{prj.location}/" .. module.Json.PublicDir .. "/**",
			"%{prj.location}/" .. module.Json.PrivateDir .. "/**"
		})
		removefiles({ ".DS_Store" })
	end
end

function PremakeUtils:CreateMissingProjectFiles(module)
	verbosef("PremakeUtils:CreateMissingProjectFiles(module)")
	os.mkdir(module.ModulePath .. module.Json.PublicDir)
	os.mkdir(module.ModulePath .. module.Json.PrivateDir)
	self:UpdateDefinesFiles(module)
	self:UpdateModuleVersionFiles(module)
	self:UpdateModuleFiles(module)
end

function PremakeUtils:UpdateDefinesFiles(module)
	verbosef("PremakeUtils:UpdateDefinesFiles(module)")
	if module.Json.Type ~= "StaticLibrary" and module.Json.Type ~= "SharedLibrary" then
		local moduleIDDefine = module:GetModuleIDDefine()

		local moduleDeps = {}
		for _, dep in ipairs(module.Json.Dependencies) do
			table.insert(moduleDeps, dep)
		end
		if module.Json.Type == "Module" or module.Json.Type == "Executable" then
			table.insert(moduleDeps, "MonsterEngine.ModuleManager")
		end

		local moduleImports = ""
		local moduleExports = "#define " .. moduleIDDefine .. "_API ME_MODULE_EXPORT\n"

		for _, dependency in ipairs(moduleDeps) do
			local dependencyModule = ModuleManager:GetModule(dependency)
			if dependencyModule and dependencyModule.Json.Type ~= "StaticLibrary" then
				local dependencyModuleIDDefine = dependencyModule:GetModuleIDDefine()
				moduleImports = moduleImports .. "#define " .. dependencyModuleIDDefine .. "_API ME_MODULE_IMPORT\n"
			end
		end

		local defines = self:FormatFileTemplate("Templates/Defines.h", {
			BuildMacrosH  = module:GetModuleID() == "MonsterEngine.ModuleManager" and "\"BuildMacros.h\"" or "<BuildMacros.h>",
			ModuleImports = moduleImports,
			ModuleExports = moduleExports
		})
		io.writefile(module.ModulePath .. module.Json.PrivateDir .. "Defines.h", defines)
	end
end

function PremakeUtils:UpdateModuleVersionFiles(module)
	verbosef("PremakeUtils:UpdateModuleVersionFiles(module)")
	if module.Json.Type == "Module" or module.Json.Type == "ModuleManager" then
		local moduleID          = module:GetModuleID()
		local moduleIDNamespace = module:GetModuleIDNamespace()
		local moduleIDDefine    = module:GetModuleIDDefine()
		local moduleIDPath      = module:GetModuleIDPath()

		local moduleVersionH    = self:FormatFileTemplate("Templates/ModuleVersion.h", {
			VersionH   = moduleID == "MonsterEngine.ModuleManager" and "\"Version.h\"" or "<Version.h>",
			Namespace  = moduleIDNamespace,
			DefinePath = moduleIDDefine,
			Version    = tostring(module.Json.Version[1]) .. ", " .. tostring(module.Json.Version[2]) .. ", " .. tostring(module.Json.Version[3])
		})
		local moduleVersionCpp = self:FormatFileTemplate("Templates/ModuleVersion.cpp", {
			Namespace = moduleIDNamespace,
			Path      = moduleIDPath
		})

		io.writefile(module.ModulePath .. module.Json.PublicDir .. moduleIDPath .. "ModuleVersion.h", moduleVersionH)
		io.writefile(module.ModulePath .. module.Json.PrivateDir .. moduleIDPath .. "ModuleVersion.cpp", moduleVersionCpp)
	end
end

function PremakeUtils:UpdateModuleFiles(module)
	verbosef("PremakeUtils:UpdateModuleFiles(module)")
	local moduleID          = module:GetModuleID()
	local moduleIDNamespace = module:GetModuleIDNamespace()
	local moduleIDDefine    = module:GetModuleIDDefine()
	local moduleIDPath      = module:GetModuleIDPath()

	if module.Json.Type == "Module" then
		local moduleHPath   = module.ModulePath .. module.Json.PublicDir .. moduleIDPath .. "Module.h"
		local moduleCppPath = module.ModulePath .. module.Json.PrivateDir .. moduleIDPath .. "Module.cpp"
		if not os.isfile(moduleHPath) then
			local moduleH = self:FormatFileTemplate("Templates/Module.h", {
				Namespace  = moduleIDNamespace,
				DefinePath = moduleIDDefine
			})
			io.writefile(moduleHPath, moduleH)

			local canOverride = true
			if os.isfile(moduleCppPath) then
				canOverride = false
				print("Module.cpp file already exists for module '" .. moduleID .. "'")
				print("Do you want to override file (Y/N)?")
				canOverride = io.read() == "Y"
			end

			if canOverride then
				local moduleCpp = self:FormatFileTemplate("Templates/Module.cpp", {
					Namespace = moduleIDNamespace,
					Path      = moduleIDPath
				})
				io.writefile(moduleCppPath, moduleCpp)
			end
		end
	elseif module.Json.Type == "Executable" then
		local entrypointCppPath = module.ModulePath .. module.Json.PrivateDir .. "Entrypoint.cpp"
		if not os.isfile(entrypointCppPath) then
			local entrypointCpp = self:FormatFileTemplate("Templates/Entrypoint.cpp", {})
			io.writefile(entrypointCppPath, entrypointCpp)
		end
	end
end

function PremakeUtils:FormatFileTemplate(templateName, vars)
	verbosef("PremakeUtils:FormatFileTemplate(%s, %s)", tostring(templateName), tostring(vars))
	local template = MonsterEngine.FileTemplates[templateName]
	if not template then
		error("Template '" .. templateName .. "' does not exist")
	end

	for k, v in pairs(vars) do
		template = template:gsub("%%%-" .. k .. "%-%%", v)
	end
	return template
end

local function ToPremakeArch(name)
	verbosef("ToPremakeArch(%s)", tostring(name))
	local lc = name:lower()
	if lc == "i386" then
		return "x86"
	elseif lc == "x86_64" or lc == "x64" then
		return "amd64"
	elseif lc == "arm32" then
		return "arm"
	else
		return lc
	end
end

local function GetHostArch()
	verbosef("GetHostArch()")
	local arch
	if PremakeUtils.Host == "windows" then
		arch = os.getenv("PROCESSOR_ARCHITECTURE")
		if arch == "x86" then
			local is64 = os.getenv("PROCESSOR_ARCHITEW6432")
			if is64 then arch = is64 end
		end
	elseif PremakeUtils.Host == "macosx" then
		arch = os.outputof("echo $HOSTTYPE")
	else
		arch = os.outputof("uname -m")
	end

	return ToPremakeArch(arch)
end

local function GetTargetArchs()
	verbosef("GetTargetArchs()")
	if _OPTIONS["target-arch"] == "host" then
		return { PremakeUtils.Arch }
	else
		local archs = string.explode(_OPTIONS["target-arch"], "%s*,%s*")
		local output = {}
		for _, arch in ipairs(archs) do
			table.insert(output, ToPremakeArch(arch))
		end
		return output
	end
end

PremakeUtils.Arch        = GetHostArch()
PremakeUtils.TargetArchs = GetTargetArchs()

term.warningColor = term.yellow
term.errorColor   = term.red
term.infoColor    = term.green

return MonsterEngine