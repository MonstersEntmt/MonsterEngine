local Engine = {
	MonsterEngine = require("MonsterEngine")
}

function Engine:SetupBuildCommands()
	self.MonsterEngine.PremakeUtils:SetupBuildCommands()
end

function Engine:HandleBuildCommands()
	return self.MonsterEngine.PremakeUtils:HandleBuildCommands()
end

function Engine:LoadEnginePackages(directory)
	self.MonsterEngine:SetEngineDirectory(directory)
	self.MonsterEngine:LoadPackages()
end

function Engine:GenerateProjectFiles()
	self:CreateWorkspace()
	local firstExecutable = true
	for _, enginePackage in ipairs(self.MonsterEngine.ModuleManager.LoadedEnginePackages) do
		for _, engineModule in ipairs(enginePackage.Modules) do
			if firstExecutable and engineModule.Json.Type == "Executable" then
				firstExecutable = false
				self:SetStartupProject(engineModule)
			end
			self:CreateProject(engineModule)
		end
	end
end

function Engine:CreateWorkspace()
	workspace("MonsterEngine")
	self.MonsterEngine.PremakeUtils:SetupWorkspace()
end

function Engine:SetStartupProject(module)
	workspace("MonsterEngine")
	startproject(module:GetModuleID())
end

function Engine:CreateProject(module)
	self.MonsterEngine.PremakeUtils:SetupModule(module)
end

return Engine