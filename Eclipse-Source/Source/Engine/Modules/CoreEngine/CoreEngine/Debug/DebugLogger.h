#pragma once

#ifdef ECLIPSED_EDITOR
#include "CoreEngine/Containers/RingBuffer.h"
#include <string>
#include "DebugMessage.h"
#include "DebugMessageFactory.h"

namespace Eclipse
{
	class DebugLogger
	{
	private:
		static inline constexpr int BufferSize = 10'000;

	public:
		friend class ConsoleWindow;

	public:
		static void AddMessage(const DebugMessage& message);

		static const RingBuffer<DebugMessage, DebugLogger::BufferSize>& GetMessages();
		static void Clear();

	private:
		static RingBuffer<DebugMessage, DebugLogger::BufferSize> messageBuffer;
	};
}

#define LOG_CODE(MESSAGE) Eclipse::DebugLogger::AddMessage(Eclipse::DebugMessageFactory::ConstructSourceCodeMessage(MESSAGE, __FILE__, __LINE__, Eclipse::DiagnosticSeverity::Info))
#define LOG_WARNING_CODE(MESSAGE) Eclipse::DebugLogger::AddMessage(Eclipse::DebugMessageFactory::ConstructSourceCodeMessage(MESSAGE, __FILE__, __LINE__, Eclipse::DiagnosticSeverity::Warning))
#define LOG_ERROR_CODE(MESSAGE) Eclipse::DebugLogger::AddMessage(Eclipse::DebugMessageFactory::ConstructSourceCodeMessage(MESSAGE, __FILE__, __LINE__, Eclipse::DiagnosticSeverity::Error))

#define LOG(MESSAGE) Eclipse::DebugLogger::AddMessage(Eclipse::DebugMessageFactory::ConstructTextMessage(MESSAGE, Eclipse::DiagnosticSeverity::Info))
#define LOG_WARNING(MESSAGE) Eclipse::DebugLogger::AddMessage(Eclipse::DebugMessageFactory::ConstructTextMessage(MESSAGE, Eclipse::DiagnosticSeverity::Warning))
#define LOG_ERROR(MESSAGE) Eclipse::DebugLogger::AddMessage(Eclipse::DebugMessageFactory::ConstructTextMessage(MESSAGE, Eclipse::DiagnosticSeverity::Error))
#else
#define LOG(MESSAGE)
#define LOG_WARNING(MESSAGE)
#define LOG_ERROR(MESSAGE)
#endif