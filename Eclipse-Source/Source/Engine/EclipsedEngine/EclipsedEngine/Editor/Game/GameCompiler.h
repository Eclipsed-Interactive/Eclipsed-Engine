#pragma once

namespace Eclipse
{
	class GameCompiler
	{
	public:
		static void CompileGame();

	private:
		static void TryCompile();
		static void GenerateForcelink();
		static void GenerateProject();
		static void Compile();
	};
}