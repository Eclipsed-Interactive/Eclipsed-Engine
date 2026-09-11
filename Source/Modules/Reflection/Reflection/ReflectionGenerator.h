#pragma once

namespace Eclipse::Reflection
{
	class TypeDescriptor;
	class ReflectionGenerator final
	{
	public:
		static void Clear();
		static void Generate();

	private:
		static void GenerateFile(const TypeDescriptor& descriptor);
		static void GenerateRegistratorFile();
	};
}