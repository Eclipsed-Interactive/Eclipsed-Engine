#ifdef ECL_EDITOR
#include "pch.h"

#include "Core/Logger/DebugLogger.h"

namespace Eclipse::Core
{
	RingBuffer<DebugMessage, DEBUG_LOGGER_BUFFER_SIZE> DebugLogger::messageBuffer;

	void DebugLogger::AddMessage(const DebugMessage& message)
	{
		messageBuffer.Push(message);
	}

	const RingBuffer<DebugMessage, DEBUG_LOGGER_BUFFER_SIZE>& DebugLogger::GetMessages()
	{
		return messageBuffer;
	}

	void DebugLogger::Clear()
	{
		messageBuffer.Clear();
	}
}
#endif