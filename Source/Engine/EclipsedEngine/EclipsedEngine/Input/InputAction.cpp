#include "pch.h"

#include "InputAction.h"

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"

#include "Input.h"

#include "CoreEngine/Macros/defines.h"

namespace Eclipse
{
	void InputAction::Update()
	{
		switch (type)
		{
		case InputActionType::Axis: value = GetAxis();
			break;
		case InputActionType::Single: value = GetSingleKey();
			break;
		case InputActionType::Combo: value = GetCombo();
			break;
		}
	}



	rapidjson::Value InputAction::Save(rapidjson::Document::AllocatorType& anAllocator)
	{
		using namespace rapidjson;

		Value action(kObjectType);
		action.AddMember(STRINGIFY(name), Value(name.c_str(), anAllocator).Move(), anAllocator);
		action.AddMember(STRINGIFY(type), (int)type, anAllocator);
		action.AddMember(STRINGIFY(buttonType), (int)buttonType, anAllocator);

		if (type == InputActionType::Single)
		{
			action.AddMember(STRINGIFY(button), (int)button, anAllocator);
		}
		else if (type == InputActionType::Combo)
		{
			Value combo(kArrayType);
			for (const Keycode::Scancode& key : comboButtons)
			{
				combo.PushBack((int)key, anAllocator);
			}

			action.AddMember(STRINGIFY(comboButtons), combo, anAllocator);
		}
		else
		{
			action.AddMember(STRINGIFY(positiveButton), (int)positiveButton, anAllocator);
			action.AddMember(STRINGIFY(negativeButton), (int)negativeButton, anAllocator);
		}

		return action;
	}

	void InputAction::Load(const rapidjson::Value& aValue)
	{
		name = aValue[STRINGIFY(name)].GetString();
		type = (InputActionType)aValue[STRINGIFY(type)].GetInt();
		buttonType = (InputActionButtonType)aValue[STRINGIFY(buttonType)].GetInt();

		if (type == InputActionType::Axis)
		{
			positiveButton = (Keycode::Scancode)aValue[STRINGIFY(positiveButton)].GetInt();
			negativeButton = (Keycode::Scancode)aValue[STRINGIFY(negativeButton)].GetInt();
		}
		else if (type == InputActionType::Combo)
		{
			const auto& comboArray = aValue[STRINGIFY(comboButtons)];
			if (comboArray.IsArray())
			{
				comboButtons.clear();
				for (auto& v : comboArray.GetArray())
				{
					comboButtons.push_back((Keycode::Scancode)v.GetInt());
				}
			}
		}
		else
		{
			button = (Keycode::Scancode)aValue[STRINGIFY(button)].GetInt();
		}
	}
	int InputAction::GetSingleKey()
	{
		return GetKey(button, buttonType);
	}

	int InputAction::GetAxis()
	{
		return GetKey(positiveButton, buttonType) - GetKey(negativeButton, buttonType);
	}
	int InputAction::GetCombo()
	{
		//for (const Keycode::Scancode& key : comboButtons)
		//{
		//	if (!Input::GetKey(key))
		//	{
		//		return 0;
		//	}
		//}

		//return true;
		return false;
	}
	bool InputAction::GetKey(Keycode::Scancode aKey, InputActionButtonType aButtonType)
	{
		//switch (buttonType)
		//{
		//case InputActionButtonType::Repeated: return Input::GetKey(aKey);
		//case InputActionButtonType::Down: return Input::GetKeyDown(aKey);
		//case InputActionButtonType::Up: return Input::GetKeyUp(aKey);
		//}

		return false;
	}
}