#pragma once

#include <string>

#include "ImGui/imgui.h"

#include "Font-Awesome/7/IconsFontAwesome7.h"
#include "Font-Awesome/7/IconsFontAwesome7Brands.h"

namespace Eclipse::Editor
{
	class AbstractWindow
	{
		friend class WindowManager;

	public:
		AbstractWindow() = delete;

		AbstractWindow(const std::string& name, const int& aId = -1);

		virtual AbstractWindow* GetNewWindow(const int& aId = -1) { return new AbstractWindow(windowName, aId); }


	public:
		virtual void Open() {};
		
		virtual void PreUpdate() {};
		virtual void Update() {};
		virtual void PostUpdate() {};

		virtual void Close() {};

	public:

		virtual const char* GetCategoryName() { return nullptr; }

		const std::string windowName = "";
		const int instanceID = 0;

	protected:
		bool myIsOpen = true;
		bool myWasLastOpen = true;

		ImGuiWindowFlags flags = 0;
	};
}

#include "CoreEngine/Macros/MacroOverloadSelector_Undef.h"