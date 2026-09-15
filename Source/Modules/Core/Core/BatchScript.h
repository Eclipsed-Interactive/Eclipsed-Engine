#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include "Core.Core.hpp"

namespace Eclipse
{
	class CORE_API BatchScript
	{
	public:
		BatchScript() = default;
		BatchScript(const std::filesystem::path& aWorkingDir, const std::string& aCommand, const std::vector<std::string>& args);
		BatchScript(const std::filesystem::path& aWorkingDir, const std::string& aCommand);

		void SetWorkingDirectory(const std::filesystem::path& aWorkingDir);
		void SetCommand(const std::string& aCommand);
		
		void AddArgument(const std::string& arg);
		void SetArguments(const std::vector<std::string>& args);
		void ClearArguments();

		bool Run(bool openNewWindow = false) const;
		bool RunCaptured(const std::string& cmd) const;
	private:
		std::filesystem::path workingDir = "";
		std::string command = "";
		std::vector<std::string> arguments;

		std::string BuildCommandString(bool openNewWindow) const;
	};
}