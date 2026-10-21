#include "../../Process.h"

#include <Windows.h>
#include <string>
#include <filesystem>

namespace Eclipse::Process
{
	bool RunAndWait(const char* executablePath, const char* const* arguments, int argumentCount, bool showConsole)
	{
		if (!executablePath || argumentCount < 0 || (argumentCount > 0 && !arguments))
		{
			return false;
		}

		std::wstring executable;
		std::wstring commandLine;

		for (int i = -1; i < argumentCount; ++i)
		{
			const char* text = i == -1 ? executablePath : arguments[i];
			if (!text)
				return false;

			const int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, nullptr, 0);

			if (length == 0)
				return false;

			std::wstring wide(length, L'\0');

			if (!MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, wide.data(), length))
			{
				return false;
			}

			wide.pop_back();

			if (i == -1)
				executable = wide;

			if (!commandLine.empty())
				commandLine += L' ';

			commandLine += L'"';
			size_t backslashes = 0;

			for (wchar_t ch : wide)
			{
				if (ch == L'\\')
				{
					++backslashes;
					continue;
				}

				commandLine.append(ch == L'"' ? backslashes * 2 + 1 : backslashes, L'\\');

				commandLine += ch;
				backslashes = 0;
			}

			commandLine.append(backslashes * 2, L'\\');
			commandLine += L'"';
		}

		STARTUPINFOW startup{};
		startup.cb = sizeof(startup);

		PROCESS_INFORMATION process{};

		if (!CreateProcessW(
			executable.c_str(), commandLine.data(),
			nullptr, nullptr, FALSE,
			showConsole ? CREATE_NEW_CONSOLE : CREATE_NO_WINDOW,
			nullptr, nullptr,
			&startup, &process))
		{
			return false;
		}

		CloseHandle(process.hThread);

		DWORD exitCode = 1;

		const bool a = WaitForSingleObject(process.hProcess, INFINITE) == WAIT_OBJECT_0;
		const bool b = GetExitCodeProcess(process.hProcess, &exitCode);
		const bool c = exitCode == 0;

		CloseHandle(process.hProcess);
		return a && b && c;
	}

	CORE_API bool RunBatchAndWait(const std::filesystem::path& batchPath, bool showConsole)
	{
		std::error_code error;
		const auto absolutePath = std::filesystem::absolute(batchPath, error);
		if (error)
			return false;

		wchar_t systemDirectory[MAX_PATH]{};
		const UINT length = GetSystemDirectoryW(systemDirectory, MAX_PATH);
		if (length == 0 || length >= MAX_PATH)
			return false;

		const std::wstring executable =
			std::wstring(systemDirectory) + L"\\cmd.exe";

		std::wstring commandLine =
			L"\"" + executable + L"\" /D /S /C \"\"" +
			absolutePath.wstring() + L"\"\"";

		const std::wstring workingDirectory =
			absolutePath.parent_path().wstring();

		STARTUPINFOW startup{};
		startup.cb = sizeof(startup);

		PROCESS_INFORMATION process{};

		if (!CreateProcessW(
			executable.c_str(),
			commandLine.data(),
			nullptr,
			nullptr,
			FALSE,
			showConsole ? CREATE_NEW_CONSOLE : CREATE_NO_WINDOW,
			nullptr,
			workingDirectory.c_str(),
			&startup,
			&process))
		{
			return false;
		}

		CloseHandle(process.hThread);

		DWORD exitCode = 1;
		const bool succeeded =
			WaitForSingleObject(process.hProcess, INFINITE) == WAIT_OBJECT_0 &&
			GetExitCodeProcess(process.hProcess, &exitCode) &&
			exitCode == 0;

		CloseHandle(process.hProcess);
		return succeeded;
	}
}