#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <filesystem>

#include "Core.Core.hpp"

namespace Eclipse::Editor
{
	enum class EventType : unsigned
	{
		Unknown = 3000,
		Modified = 0x00000003,
		FileAdded = 0x00000001,
		FileRemoved = 0x00000002,
		RenamedOld = 0x00000004,
		RenamedNew = 0x00000005,
		Moved = 200,
	};

	struct FileWatcherEvent
	{
		std::string root;
		std::string path;
		int action;
	};

	struct WatchedDirectory {
		std::thread thread;

		bool running = false;

		std::vector<std::function<void(const FileWatcherEvent&)>> onChangedEvents;
		std::vector<FileWatcherEvent> events;

		~WatchedDirectory() {
			thread.join();
		}
	};

	class CORE_API FileWatcher final
	{
	public:
		FileWatcher() = delete;
		~FileWatcher() = delete;

		static void WatchPath(const std::filesystem::path& path, WatchedDirectory& dir);
		static void SubscribeToPath(const std::filesystem::path& path, std::function<void(const FileWatcherEvent&)> callback);

	private:
		static void InvokeEvents(WatchedDirectory& dir);

	private:
		static inline std::unordered_map<std::filesystem::path, WatchedDirectory> watchedDirectories;
		static inline std::mutex watchedDirectoriesMutex;
	};
}