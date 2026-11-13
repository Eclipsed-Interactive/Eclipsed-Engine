#pragma once

#include <filesystem>
#include "SettingsScope.h"

namespace Eclipse::Settings
{
	class ISettingsBlock
	{
	public:
		~ISettingsBlock() = default;

	public:
		virtual const char* Name() const = 0;
		virtual SettingsScope Scope() const = 0;

	public:
		virtual void Load(const std::filesystem::path& aPath) = 0;
		virtual void Save(const std::filesystem::path& aPath) const = 0;
	};
}