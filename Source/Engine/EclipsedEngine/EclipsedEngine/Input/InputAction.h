#pragma once

#include <vector>
#include <string>
#include "rapidjson/document.h"

#include "CoreEngine/Input/Keycodes.h"

namespace Eclipse
{
	enum class InputActionType
	{
		Axis, Single, Combo
	};

	enum class InputActionButtonType
	{
		Repeated, Down, Up
	};

	struct InputAction
	{
		friend class InputMapper;

		InputAction() = default;
		InputAction(const std::string& actionName) : name(actionName) {}

		InputActionType type = {};
		InputActionButtonType buttonType = {};

		Keycode::Scancode positiveButton = {};
		Keycode::Scancode negativeButton = {};

		Keycode::Scancode button = {};
		
		std::vector<Keycode::Scancode> comboButtons;

		std::string name;
		
	private:
		int value = 0;

	private:
		void Update();

		rapidjson::Value Save(rapidjson::Document::AllocatorType& anAllocator);
		void Load(const rapidjson::Value& aValue);

		int GetSingleKey();
		int GetAxis();
		int GetCombo();

		bool GetKey(Keycode::Scancode aKey, InputActionButtonType aButtonType);
	};
}