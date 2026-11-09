#include "pch.h"

#include "Logger/DebugMessageFactory.h"

#include <chrono>
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

	DebugMessage DebugMessageFactory::ConstructSourceCodeMessage(const std::string& message, const std::string& file, DiagnosticSeverity severity)
	{
		DebugMessage m = ConstructDefaultMessage(message, severity);
		m.diagnosticKind = DiagnosticKind::SourceCode;
		m.file = file;

		return m;
	}

	DebugMessage DebugMessageFactory::ConstructDefaultMessage(const std::string& message, DiagnosticSeverity severity)
	{
		DebugMessage m;
		m.message = message;
		m.diagnosticSeverity = severity;

		return m;
	}
}