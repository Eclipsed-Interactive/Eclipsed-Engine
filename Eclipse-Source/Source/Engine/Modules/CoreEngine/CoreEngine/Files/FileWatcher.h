#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>

#include <Windows.h>

namespace Eclipse::Editor
{
	enum class EventType : unsigned
	{
		Unknown = 3000,
		Modified = FILE_ACTION_MODIFIED,
		FileAdded = FILE_ACTION_ADDED,
		FileRemoved = FILE_ACTION_REMOVED,
		RenamedOld = FILE_ACTION_RENAMED_OLD_NAME,
		RenamedNew = FILE_ACTION_RENAMED_NEW_NAME,
		Moved = 200,
	};

	struct FileWatcherEvent
	{
		std::string path;
		int action;
	};

	struct WatchedDirectory {
		std::thread thread;
		std::function<void(const FileWatcherEvent&)> onChangedEvent;

		std::vector<FileWatcherEvent> events;
		std::string watchPath;
	};

	class FileWatcher final
	{
	public:
		FileWatcher() = delete;
		~FileWatcher() = delete;

		static void WatchPath(WatchedDirectory& aDir);
		static void Stop();

		static void Subscribe(const std::string& name, std::function<void(const FileWatcherEvent&)> func);
		static void Unsubscribe(const std::string& name);
		static void InvokeEvents();

	private:
		static inline std::unordered_map<std::string, WatchedDirectory> watchedDirectories;
		static inline std::mutex watchedDirectoriesMutex;
	};
}