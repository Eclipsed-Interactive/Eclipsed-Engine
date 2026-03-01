#ifdef ECLIPSED_EDITOR
#include "pch.h"

#include "DebugLogger.h"

namespace Eclipse
{
	RingBuffer<DebugMessage, DebugLogger::BufferSize> DebugLogger::messageBuffer;

	void DebugLogger::AddMessage(const DebugMessage& message)
	{
		messageBuffer.Push(message);
	}

	const RingBuffer<DebugMessage, DebugLogger::BufferSize>& DebugLogger::GetMessages()
	{
		return messageBuffer;
	}

	void DebugLogger::Clear()
	{
		messageBuffer.Clear();
	}
}
#endif