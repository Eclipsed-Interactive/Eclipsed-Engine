#ifdef ECL_EDITOR
#include "pch.h"

#include "Core/Logger/DebugLogger.h"

namespace Eclipse::Core
{
	DebugLoggerBuffer DebugLogger::GameBuffer;
	DebugLoggerBuffer DebugLogger::EngineBuffer;

	void DebugLogger::AddGameLogMessage(const DebugMessage& message, int line)
	{
		GameBuffer.AddMessage(message, line);
	}


	void DebugLogger::AddEngineLogMessage(const DebugMessage& message, int line)
	{
		EngineBuffer.AddMessage(message, line);
	}

	const std::unordered_set<DebugMessage, DebugMessageHash>& DebugLogger::GetUniqueGameMessages()
	{
		return GameBuffer.GetUniqueMessages();
	}

	const RingBuffer<DebugMessageInstance, LOGGER_SIZE>& DebugLogger::GetGameMessageInstances()
	{
		return GameBuffer.GetMessageInstances();
	}
	
	const DebugLoggerBuffer& DebugLogger::GetGameBuffer()
	{
		return GameBuffer;
	}

	const std::unordered_set<DebugMessage, DebugMessageHash>& DebugLogger::GetUniqueEngineMessages()
	{
		return EngineBuffer.GetUniqueMessages();
	}

	const RingBuffer<DebugMessageInstance, LOGGER_SIZE>& DebugLogger::GetEngineMessageInstances()
	{
		return EngineBuffer.GetMessageInstances();
	}

	const DebugLoggerBuffer& DebugLogger::GetEngineBuffer()
	{
		return EngineBuffer;
	}

	void DebugLogger::ClearBuffers()
	{
		EngineBuffer.Clear();
		GameBuffer.Clear();
	}

	void DebugLogger::ClearGameBuffer()
	{
		GameBuffer.Clear();
	}

	void DebugLogger::ClearEngineBuffer()
	{
		EngineBuffer.Clear();
	}
}
#endif