#include "pch.h"

#include "DebugMessageFactory.h"

#include <ctime>
#include <sstream>
#include <iomanip>

namespace Eclipse
{
	DebugMessage DebugMessageFactory::ConstructTextMessage(const std::string& message, DiagnosticSeverity severity)
	{
		DebugMessage m = ConstructDefaultMessage(message, severity);
		m.diagnosticKind = DiagnosticKind::Text;

		return m;
	}

	DebugMessage DebugMessageFactory::ConstructSourceCodeMessage(const std::string& message, const std::string& file, int line, DiagnosticSeverity severity)
	{
		DebugMessage m = ConstructDefaultMessage(message, severity);
		m.diagnosticKind = DiagnosticKind::SourceCode;
		m.line = line;
		m.file = file;

		return m;
	}

	DebugMessage DebugMessageFactory::ConstructDefaultMessage(const std::string& message, DiagnosticSeverity severity)
	{
		DebugMessage m;
		m.message = message;
		m.diagnosticSeverity = severity;
		m.timeStamp = GetTimestamp();

		return m;
	}

	std::string DebugMessageFactory::GetTimestamp()
	{
		const auto now = std::time(nullptr);
		const auto currentTime = std::localtime(&now);

		std::stringstream timeString;
		timeString << std::put_time(currentTime, "%H:%M:%S");

		return timeString.str();
	}
}