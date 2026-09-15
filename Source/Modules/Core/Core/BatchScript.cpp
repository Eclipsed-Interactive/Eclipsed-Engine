#include "BatchScript.h"

#include <windows.h>

#include "Logger/DebugLogger.h"
#include <thread>


namespace Eclipse
{
	BatchScript::BatchScript(const std::filesystem::path& aWorkingDir, const std::string& aCommand, const std::vector<std::string>& args)
		: workingDir(aWorkingDir), command(aCommand), arguments(args)
	{

	}

	BatchScript::BatchScript(const std::filesystem::path& aWorkingDir, const std::string& aCommand)
		: workingDir(aWorkingDir), command(aCommand)
	{

	}

	void BatchScript::SetWorkingDirectory(const std::filesystem::path& aWorkingDir)
	{
		workingDir = aWorkingDir;
	}

	void BatchScript::SetCommand(const std::string& aCommand)
	{
		command = aCommand;
	}

	void BatchScript::AddArgument(const std::string& arg)
	{
		arguments.push_back(arg);
	}

	void BatchScript::SetArguments(const std::vector<std::string>& args)
	{
		arguments = args;
	}

	void BatchScript::ClearArguments()
	{
		arguments.clear();
	}

	bool BatchScript::Run(bool openNewWindow) const
	{
		std::string cmd = BuildCommandString(openNewWindow);

		if(openNewWindow) return system(cmd.c_str()) == 0;

		return RunCaptured(cmd);
	}


    bool BatchScript::RunCaptured(const std::string& cmd) const
    {
        SECURITY_ATTRIBUTES sa{};
        sa.nLength = sizeof(sa);
        sa.bInheritHandle = TRUE;

        HANDLE outRead = nullptr, outWrite = nullptr;
        HANDLE errRead = nullptr, errWrite = nullptr;

        if (!CreatePipe(&outRead, &outWrite, &sa, 0)) return false;
        if (!CreatePipe(&errRead, &errWrite, &sa, 0))
        {
            CloseHandle(outRead);
            CloseHandle(outWrite);
            return false;
        }

        SetHandleInformation(outRead, HANDLE_FLAG_INHERIT, 0);
        SetHandleInformation(errRead, HANDLE_FLAG_INHERIT, 0);

        STARTUPINFOA si{};
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdOutput = outWrite;
        si.hStdError = errWrite;

        PROCESS_INFORMATION pi{};

        std::string fullCmd = "cmd /C " + cmd;

        if (!CreateProcessA(
            nullptr,
            fullCmd.data(),
            nullptr,
            nullptr,
            TRUE,
            0,
            nullptr,
            workingDir.string().c_str(),
            &si,
            &pi))
        {
            CloseHandle(outRead);
            CloseHandle(outWrite);
            CloseHandle(errRead);
            CloseHandle(errWrite);
            return false;
        }

        CloseHandle(outWrite);
        CloseHandle(errWrite);

        char buffer[4096];
        DWORD bytesRead = 0;

        bool running = true;

        std::thread stdoutThread([&]()
            {
                while (ReadFile(outRead, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0)
                {
                    std::string msg(buffer, bytesRead);
                    if (msg == "\n" || msg == "\0" || msg == "\r\n") continue;

                    if (!msg.empty())
                        CORE_LOG(msg);
                }
            });

        std::thread stderrThread([&]()
            {
                while (ReadFile(errRead, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0)
                {
                    std::string msg(buffer, bytesRead);
                    if (msg == "\n" || msg == "\0") continue;

                    if (!msg.empty())
                        CORE_LOG_ERROR(msg);
                }
            });

        WaitForSingleObject(pi.hProcess, INFINITE);

        stdoutThread.join();
        stderrThread.join();

        DWORD exitCode = 1;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(outRead);
        CloseHandle(errRead);

        return exitCode == 0;
    }

	std::string BatchScript::BuildCommandString(bool openNewWindow) const
	{
		std::string cmd = "cd /d \"" + workingDir.generic_string() + "\" && ";

		if (openNewWindow)
			cmd += "start ";

		cmd += command;

		for (const std::string& arg : arguments)
			cmd += " " + arg;

		return cmd;
	}
}