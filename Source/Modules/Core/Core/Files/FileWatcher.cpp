#include "pch.h"
#include "Core/Files/FileWatcher.h"

#include <Windows.h>

namespace Eclipse::Editor
{
	void FileWatcher::WatchPath(const std::filesystem::path& path, WatchedDirectory& dir)
	{
		std::wstring wAssetPath = path.wstring();

		HANDLE hDir = CreateFileW(
			wAssetPath.c_str(),
			FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			nullptr,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS,
			nullptr
		);

		if (hDir == INVALID_HANDLE_VALUE)
			return;

		alignas(DWORD) char buffer[4096];
		DWORD bytesReturned = 0;

		while (true)
		{
			BOOL success = ReadDirectoryChangesW(
				hDir,
				buffer,
				sizeof(buffer),
				TRUE,
				FILE_NOTIFY_CHANGE_FILE_NAME |
				FILE_NOTIFY_CHANGE_DIR_NAME |
				FILE_NOTIFY_CHANGE_LAST_WRITE,
				&bytesReturned,
				nullptr,
				nullptr
			);

			if (!success)
				continue;

			auto* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);

			do
			{
				std::wstring fileName(
					info->FileName,
					info->FileNameLength / sizeof(WCHAR)
				);

				std::filesystem::path fullPath =
					std::filesystem::path(path) / fileName;

				// ONLY STORE EVENT — NO CALLBACKS HERE
				{
					std::lock_guard<std::mutex> lock(watchedDirectoriesMutex);

					dir.events.push_back({
						path.string(),
						fullPath.string(),
						static_cast<int>(info->Action)
						});
				}

				if (info->NextEntryOffset == 0)
					break;

				info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
					reinterpret_cast<BYTE*>(info) + info->NextEntryOffset
					);

			} while (true);

			InvokeEvents(dir);
		}

		CloseHandle(hDir);
	}

	void FileWatcher::SubscribeToPath(
		const std::filesystem::path& path,
		std::function<void(const FileWatcherEvent&)> callback)
	{
		std::lock_guard<std::mutex> lock(watchedDirectoriesMutex);

		WatchedDirectory& dir = watchedDirectories[path];
		dir.onChangedEvents.push_back(callback);

		if (!dir.thread.joinable())
		{
			dir.thread = std::thread(
				&FileWatcher::WatchPath,
				path,
				std::ref(dir)
			);
		}
	}

	void FileWatcher::InvokeEvents(WatchedDirectory& dir)
	{
		// MOVE EVENTS OUT (no holding lock while processing)
		std::vector<FileWatcherEvent> events;
		std::vector<std::function<void(const FileWatcherEvent&)>> callbacks;

		{
			std::lock_guard<std::mutex> lock(watchedDirectoriesMutex);

			events = std::move(dir.events);
			dir.events.clear();

			callbacks = dir.onChangedEvents;
		}

		// SAFE: main-thread or controlled dispatch point
		for (auto& event : events)
		{
			for (auto& cb : callbacks)
			{
				cb(event);
			}
		}
	}
}