if not MonsterEngineProject then
	local engine = require("Programs/Premake/Scripts/Engine")
	engine:SetupBuildCommands()
	if not engine:HandleBuildCommands() then
		engine:LoadEnginePackages(os.getcwd() .. "/")
		engine:GenerateProjectFiles()
	end
end