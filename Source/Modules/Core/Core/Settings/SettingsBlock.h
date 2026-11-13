#pragma once

#include "ISettingsBlock.h"
#include "Core/PathManager.h"
#include "SettingsRegistry.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Eclipse::Settings
{
	template<typename T>
	class SettingsBlock : public ISettingsBlock
	{
	public:
		SettingsBlock(const char* aName, SettingsScope aScope);
		~SettingsBlock() = default;

	public:
		[[nodiscard]] const char* Name() const  override { return myName.c_str(); };
		[[nodiscard]] SettingsScope Scope() const override { return myScope; };

	public:
		[[nodiscard]] T& Get() { return data; }

	public:
		void Load(const std::filesystem::path& aPath) override;
		void Save(const std::filesystem::path& aPath) const override;

	private:
		T data{};

		std::string myName = "";
		SettingsScope myScope = SettingsScope::User;
	};
}


namespace Eclipse::Settings
{
	namespace {
		std::filesystem::path GetPathFromScope(SettingsScope scope)
		{
			return scope == SettingsScope::User ? PathManager::GetProjectLocalPath() / "Settings" : PathManager::GetSettingsPath();
		}
	}

	template<typename T>
	inline SettingsBlock<T>::SettingsBlock(const char* aName, SettingsScope aScope)
		: myName(aName), myScope(aScope)
	{
		SettingsRegistry::Register(*this);
	}

	template<typename T>
	inline void SettingsBlock<T>::Load(const std::filesystem::path& aPath)
	{
		try
		{
			std::ifstream in(aPath);
			cereal::JSONInputArchive archive(in);
			archive(cereal::make_nvp(myName, data));
		}
		catch (const std::exception&)
		{
			LOG_WARNING("Settings dont exist.");
			data = {};
		}
	}

	template<typename T>
	inline void SettingsBlock<T>::Save(const std::filesystem::path& aPath) const
	{
		std::filesystem::create_directories(aPath.parent_path());

		try
		{
			std::ofstream out(aPath);
			cereal::JSONOutputArchive archive(out);
			archive(cereal::make_nvp(myName, data));
		}
		catch (const std::exception&)
		{
			LOG_WARNING("Failed to save settings.");
		}
	}
}