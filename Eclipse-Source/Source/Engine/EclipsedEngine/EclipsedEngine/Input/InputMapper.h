#pragma once

#include "CoreEngine/Input/Keycodes.h"

#include <string>
#include <unordered_map>
#include <vector>
#include "rapidjson/document.h"

#include "InputAction.h"

namespace Eclipse
{
	class InputMapper
	{
	public:
		friend class InputEditorWindow;

		static void Save();
		static void Load();

		static void Update();

		static std::unordered_map<std::string, InputAction>& GetInputMap() { return map; }

	public:
		static int ReadValue(const std::string& actionName);

		static InputAction& AddAction(const std::string& actionName);



	private:
		static inline std::unordered_map<std::string, InputAction> map;
	};
}