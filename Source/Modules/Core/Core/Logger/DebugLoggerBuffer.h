#pragma once

#include <chrono>
#include <unordered_set>
#include "DebugMessage.h"
#include "Core/Containers/RingBuffer.h"

#define LOGGER_SIZE 40'000

namespace Eclipse::Core
{
	struct DebugLoggerBuffer
	{
	public:
		const std::unordered_set<DebugMessage, DebugMessageHash>& GetUniqueMessages()const;
		const RingBuffer<DebugMessageInstance, LOGGER_SIZE>& GetMessageInstances() const;

		void AddMessage(const DebugMessage& message, int line);

		void Clear();

		static std::chrono::seconds GetTimestamp();

	private:
		std::unordered_set<DebugMessage, DebugMessageHash> uniqueMessages;
		RingBuffer<DebugMessageInstance, LOGGER_SIZE> messageInstanceBuffer;
	};

	inline const std::unordered_set<DebugMessage, DebugMessageHash>& DebugLoggerBuffer::GetUniqueMessages() const
	{
		return uniqueMessages;
	}

	inline const RingBuffer<DebugMessageInstance, LOGGER_SIZE>& DebugLoggerBuffer::GetMessageInstances() const
	{
		return messageInstanceBuffer;
	}

	inline void DebugLoggerBuffer::AddMessage(const DebugMessage& message, int line)
	{
		auto [it, inserted] = uniqueMessages.emplace(message);
		it->count++;
		messageInstanceBuffer.Push(DebugMessageInstance{ .timeStamp = GetTimestamp(), .line = line, .message = &(*it) });
	}

	inline void DebugLoggerBuffer::Clear()
	{
		uniqueMessages.clear();
		messageInstanceBuffer.Clear();
	}

	inline std::chrono::seconds DebugLoggerBuffer::GetTimestamp()
	{
		const auto local = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() }.get_local_time();
		return std::chrono::floor<std::chrono::seconds>(local - std::chrono::floor<std::chrono::days>(local));
	}
}
