#include "Core/Settings/ConfigManager.h"

#include "Core/Settings/Presets/BuildPresets.h"

namespace Eclipse::Configs
{
	std::unordered_map<std::string, Config> ConfigManager::configs;

	void ConfigManager::Init()
	{
		configs["BuildPresets"] = BuildPresets::Default();
		LoadOrCreate(configs["BuildPresets"], (PathManager::GetSettingsPath() / "build.conf").generic_string().c_str());
		configs["BuildPresets"].GenerateHeaderFile();
	}

	Config& ConfigManager::GetConfig(const char* name)
	{
		const std::string key = name;

		auto it = configs.find(key);
		if (it != configs.end()) return configs[key];

		throw std::runtime_error("The config does not exist in map.");
	}

	void ConfigManager::LoadOrCreate(Config& conf, const char* configPath)
	{
		if (std::filesystem::exists(configPath))
			conf.Load(configPath);
		else
			conf.Save(configPath);
	}
}