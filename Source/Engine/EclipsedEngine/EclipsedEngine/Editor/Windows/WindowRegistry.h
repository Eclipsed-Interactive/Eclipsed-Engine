#pragma once

#include <unordered_map>
#include <string>


namespace Eclipse::Editor
{
	class AbstractWindow;
	class WindowRegistry 
	{
	public:
		template<typename T>
		static void RegisterWindow(const std::string& name);

		static AbstractWindow* GetWindow(const std::string& name);

		static const std::unordered_map<std::string, AbstractWindow*>& GetWindows();

	private:
		static inline std::unordered_map<std::string, AbstractWindow*> registeredWindows;
	};

	template<typename T>
	inline void WindowRegistry::RegisterWindow(const std::string& name)
	{
		registeredWindows[name] = new T();
	}
}