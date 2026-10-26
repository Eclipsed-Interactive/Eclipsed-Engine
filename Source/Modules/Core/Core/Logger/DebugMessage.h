#pragma once

#include <string>

#include "Core.Core.hpp"

namespace Eclipse
{
	enum class CORE_API DiagnosticSeverity
	{
		Info, 
		Warning,
		Error
	};

	enum class CORE_API DiagnosticKind
	{
		Text,
		SourceCode 
	};

	struct CORE_API DebugMessage
	{
		DiagnosticSeverity diagnosticSeverity = DiagnosticSeverity::Info;
		DiagnosticKind diagnosticKind = DiagnosticKind::Text;

		std::string message = "";
		std::string file;

		std::string timeStamp = "";
		
		int line = -1;
	};
}

