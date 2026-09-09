#pragma once

#include <string>
#include <unordered_map>
#include <Windows.h>

namespace Eclipse
{
	using Func = void(*)();
	using dawd = void(*)(int d);

	class Plugin
	{
	public:
		Plugin(const char* source);

	public:
		template<typename TFunc = Func>
		TFunc GetMethod(const char* funcName);

		Func GetInitFunc() const;

		bool Succeded() const;

		const char* GetName() const;

	private:
		void* dllModule = nullptr;
		Func initFunc = nullptr;

		bool succeded = false;

		std::string name = "";
	};

	template<typename TFunc>
	inline TFunc Plugin::GetMethod(const char* funcName)
	{
		TFunc func = (TFunc)GetProcAddress((HMODULE)dllModule, funcName);

		succeded = func != nullptr;
		return func;
	}
}