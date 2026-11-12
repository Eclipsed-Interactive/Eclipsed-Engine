#pragma once

#ifndef CHANGE_THIS_TO_A_SHIPPING_DEFINE
#include "Core/Containers/RingBuffer.h"
#include <string>
#include <chrono>
#include <unordered_set>
#include "DebugMessage.h"
#include "DebugMessageFactory.h"
#include "DebugLoggerBuffer.h"

#include "Core.Core.hpp"

namespace Eclipse::Core
{
	class CORE_API DebugLogger
	{
	public:
		friend class ConsoleWindow;

	public:
		static void AddGameLogMessage(const DebugMessage& message, int line);
		static void AddEngineLogMessage(const DebugMessage& message, int line);

	public:
		static const std::unordered_set<DebugMessage, DebugMessageHash>& GetUniqueGameMessages();
		static const RingBuffer<DebugMessageInstance, LOGGER_SIZE>& GetGameMessageInstances();
		static const DebugLoggerBuffer& GetGameBuffer();

		static const std::unordered_set<DebugMessage, DebugMessageHash>& GetUniqueEngineMessages();
		static const RingBuffer<DebugMessageInstance, LOGGER_SIZE>& GetEngineMessageInstances();
		static const DebugLoggerBuffer& GetEngineBuffer();

		static void ClearBuffers();
		static void ClearGameBuffer();
		static void ClearEngineBuffer();

	private:
		static DebugLoggerBuffer GameBuffer;
		static DebugLoggerBuffer EngineBuffer;
	};
}

#endif