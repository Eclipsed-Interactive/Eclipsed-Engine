#pragma once

#include "Core.Core.hpp"

#include <string>
#include <chrono>

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

		int line = -1;

		std::string message = "";
		std::string file = "";

		bool operator==(const DebugMessage&) const = default;
	};

	struct CORE_API DebugMessageInstance
	{
		// Time since midnight.
		std::chrono::seconds timeStamp{};

		const DebugMessage* message;
	};

	struct DebugMessageHash
	{
		size_t operator()(const DebugMessage& m) const noexcept
		{
			size_t hash = std::hash<std::string>{}(m.message);

			auto combine = [&hash](size_t v)
				{
					hash ^= v + 0x9e3779b97f4a7c15ull + (hash << 6) + (hash >> 2);
				};

			combine(std::hash<std::string>{}(m.file));
			combine(std::hash<int>{}(m.line));
			combine(static_cast<size_t>(m.diagnosticSeverity));
			combine(static_cast<size_t>(m.diagnosticKind));

			return hash;
		}
	};
}

