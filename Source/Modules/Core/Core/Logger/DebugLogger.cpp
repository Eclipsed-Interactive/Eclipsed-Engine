#ifdef ECL_EDITOR
#include "pch.h"

#include "Core/Logger/DebugLogger.h"

namespace Eclipse::Core
{
	std::unordered_set<DebugMessage, DebugMessageHash> DebugLogger::uniqueMessages;
	RingBuffer<DebugMessageInstance, DEBUG_LOGGER_BUFFER_SIZE> DebugLogger::messageInstanceBuffer;

	void DebugLogger::AddMessage(const DebugMessage& message)
	{
		auto [it, inserted] = uniqueMessages.emplace(message);
		messageInstanceBuffer.Push(DebugMessageInstance{ .timeStamp = GetTimestamp(), .message = &(*it) });
	}

	const RingBuffer<DebugMessageInstance, DEBUG_LOGGER_BUFFER_SIZE>& DebugLogger::GetMessages()
	{
		return messageInstanceBuffer;
	}

	void DebugLogger::Clear()
	{
		uniqueMessages.clear();
		messageInstanceBuffer.Clear();
	}

	std::chrono::seconds DebugLogger::GetTimestamp()
	{
		const auto local = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() }.get_local_time();
		return std::chrono::floor<std::chrono::seconds>(local - std::chrono::floor<std::chrono::days>(local));
	}
}
#endif