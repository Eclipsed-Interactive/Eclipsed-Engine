#pragma once

#ifdef ECL_EDITOR
#include "Core/Containers/RingBuffer.h"
#include <string>
#include <chrono>
#include <unordered_set>
#include "DebugMessage.h"
#include "DebugMessageFactory.h"

#include "Core.Core.hpp"

namespace Eclipse::Core
{
#define DEBUG_LOGGER_BUFFER_SIZE 40'000

	class CORE_API DebugLogger
	{
	public:
		friend class ConsoleWindow;

	public:
		static void AddMessage(const DebugMessage& message);

		static const RingBuffer<DebugMessageInstance, DEBUG_LOGGER_BUFFER_SIZE>& GetMessages();
		static void Clear();

	private:
		static std::chrono::seconds GetTimestamp();

	private:
		static std::unordered_set<DebugMessage, DebugMessageHash> uniqueMessages;
		static RingBuffer<DebugMessageInstance, DEBUG_LOGGER_BUFFER_SIZE> messageInstanceBuffer;
	};
}

#define LOG(MESSAGE) Eclipse::Core::DebugLogger::AddMessage(Eclipse::DebugMessageFactory::ConstructSourceCodeMessage(MESSAGE, __FILE__, __LINE__, Eclipse::DiagnosticSeverity::Info))
#define LOG_WARNING(MESSAGE) Eclipse::Core::DebugLogger::AddMessage(Eclipse::DebugMessageFactory::ConstructSourceCodeMessage(MESSAGE, __FILE__, __LINE__, Eclipse::DiagnosticSeverity::Warning))
#define LOG_ERROR(MESSAGE) Eclipse::Core::DebugLogger::AddMessage(Eclipse::DebugMessageFactory::ConstructSourceCodeMessage(MESSAGE, __FILE__, __LINE__, Eclipse::DiagnosticSeverity::Error))

#else
#define LOG(MESSAGE)
#define LOG_WARNING(MESSAGE)
#define LOG_ERROR(MESSAGE)
#endif