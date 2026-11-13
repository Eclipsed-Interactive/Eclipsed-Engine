#include "SettingsRegistry.h"

#include "ISettingsBlock.h"

namespace Eclipse::Settings
{
	std::vector<ISettingsBlock*>& SettingsRegistry::GetList()
	{
		static std::vector<ISettingsBlock*> list;
		return list;
	}

	void SettingsRegistry::Register(ISettingsBlock& aBlock)
	{
		GetList().push_back(&aBlock);
	}

	void SettingsRegistry::SaveAll()
	{
		for (const ISettingsBlock* block : GetList())
			SaveOne(block);
	}

	void SettingsRegistry::SaveOne(const ISettingsBlock* aBlock)
	{
		const std::filesystem::path savepath = GetSavePathFromScope(aBlock->Scope()) / aBlock->Name();
		aBlock->Save(savepath);
	}

	void SettingsRegistry::LoadAll()
	{
		for (ISettingsBlock* block : GetList())
			LoadOne(block);
	}

	void SettingsRegistry::LoadOne(ISettingsBlock* aBlock)
	{
		const std::filesystem::path savepath = GetSavePathFromScope(aBlock->Scope()) / aBlock->Name();
		aBlock->Load(savepath);
	}

	std::filesystem::path SettingsRegistry::GetSavePathFromScope(SettingsScope aScope)
	{
		switch (aScope)
		{
		case Eclipse::Settings::SettingsScope::User: return PathManager::GetLocalSettingsPath();
		case Eclipse::Settings::SettingsScope::Project: return PathManager::GetSettingsPath();
		}
	}
}
