#pragma once

#include "DebugMessage.h"

#include "Core.Core.hpp"

namespace Eclipse
{
	class CORE_API DebugMessageFactory
	{
	public:
		static DebugMessage ConstructTextMessage(const std::string& message, DiagnosticSeverity severity);
		static DebugMessage ConstructSourceCodeMessage(const std::string& message, const std::string& file, int line, DiagnosticSeverity severity);

	private:
		static DebugMessage ConstructDefaultMessage(const std::string& message, DiagnosticSeverity severity);

		static std::string GetTimestamp();
	};
}