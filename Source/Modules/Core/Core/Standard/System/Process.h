#pragma once

#include "Core.Core.hpp"

#include <filesystem>

namespace Eclipse::Process
{
	/// <summary>
	/// Runs an executable and waits until it finishes.
	/// Works with bat files as well, but executable needs to be that cmd path and argument the bat file path.
	/// </summary>
	/// <param name="executablePath">The path to the executable. (.exe)</param>
	/// <param name="arguments">The arguments for the executable.</param>
	/// <param name="argumentCount">The amount of arguments passed.</param>
	/// <returns>True if the process succeded and false otherwise.</returns>
	CORE_API bool RunAndWait(const char* executablePath, const char* const* arguments, int argumentCount, bool showConsole = false);

	CORE_API bool RunBatchAndWait(const std::filesystem::path& batchPath, bool showConsole = false);

	/// <summary>
	/// Runs a batch file without a console window and waits until it finishes.
	/// Stdout and stderr are written to logPath (overwritten), stdin is NUL so 'pause' does not block.
	/// </summary>
	/// <param name="batchPath">The path to the batch file. Its directory is used as the working directory.</param>
	/// <param name="logPath">The file that receives all output from the batch file.</param>
	/// <param name="exitCode">Receives the exit code of the batch file.</param>
	/// <returns>True if the batch file ran and returned exit code 0, false otherwise.</returns>
	CORE_API bool RunBatchAndWait(const std::filesystem::path& batchPath, const std::filesystem::path& logPath, unsigned long& exitCode);
}