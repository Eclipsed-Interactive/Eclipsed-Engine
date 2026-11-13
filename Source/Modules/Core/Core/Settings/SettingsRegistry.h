#pragma once

#include "ISettingsBlock.h"

#include "Core.Core.hpp"

namespace Eclipse::Settings
{
	class CORE_API SettingsRegistry final
	{
	public:
		static std::vector<ISettingsBlock*>& GetList();
		static void Register(ISettingsBlock&);

		static void SaveAll();
		static void SaveOne(const ISettingsBlock* aBlock);

		static void LoadAll();
		static void LoadOne(ISettingsBlock* aBlock);

	private:
		static std::filesystem::path GetSavePathFromScope(SettingsScope aScope);
	};
}