#include "InputEditorWindow.h"

#include "HierarchyWindow.h"

#include "EntityEngine/ComponentManager.h"

#include "EclipsedEngine/Input/InputMapper.h"
#include "EclipsedEngine/Input/Input.h"

#include "rapidjson/stringbuffer.h"

#include "ImGui/imgui.h"

#include <filesystem>

#include <rapidjson/document.h>

#include "CoreEngine/MainSingleton.h"

namespace Eclipse::Editor
{
	struct inputNameChange
	{
		std::string inputNamePrev;
		std::string inputNameNew;

		InputAction inputAction;
	};

	void ActionType(std::pair<const std::string, InputAction>& aInput)
	{
		ImGui::Text("Action Type");
		ImGui::SameLine();

		std::string currentPopupName;

		switch (aInput.second.type)
		{
		case InputActionType::Axis:
			currentPopupName = ("Axis ##" + aInput.first).c_str();
			break;
		case InputActionType::Single:
			currentPopupName = ("Single ##" + aInput.first).c_str();
			break;
		case InputActionType::Combo:
			currentPopupName = ("Combo ##" + aInput.first).c_str();
			break;
		default:
			currentPopupName = ("NONE ##" + aInput.first).c_str();
			break;
		}

		std::string inputType = ("##ActionTypeChange" + aInput.first).c_str();

		if (ImGui::Button(currentPopupName.c_str()))
			ImGui::OpenPopup(inputType.c_str());

		if (ImGui::BeginPopup(inputType.c_str()))
		{
			if (ImGui::Selectable(("Axis ##" + aInput.first).c_str()))
				aInput.second.type = InputActionType::Axis;

			if (ImGui::Selectable(("Single ##" + aInput.first).c_str()))
				aInput.second.type = InputActionType::Single;

			if (ImGui::Selectable(("Combo ##" + aInput.first).c_str()))
				aInput.second.type = InputActionType::Combo;

			ImGui::EndPopup();
		}
	}

	void ButtonType(std::pair<const std::string, InputAction>& aInput)
	{
		ImGui::Text("Button Type");
		ImGui::SameLine();

		std::string currentPopupName;

		switch (aInput.second.buttonType)
		{
		case InputActionButtonType::Repeated:
			currentPopupName = ("Repeated ##" + aInput.first).c_str();
			break;
		case InputActionButtonType::Down:
			currentPopupName = ("Down ##" + aInput.first).c_str();
			break;
		case InputActionButtonType::Up:
			currentPopupName = ("Up ##" + aInput.first).c_str();
			break;
		default:
			currentPopupName = ("NONE ##" + aInput.first).c_str();
			break;
		}

		std::string inputType = ("##ButtonTypeChange" + aInput.first).c_str();

		if (ImGui::Button(currentPopupName.c_str()))
			ImGui::OpenPopup(inputType.c_str());

		if (ImGui::BeginPopup(inputType.c_str()))
		{
			if (ImGui::Selectable(("Repeated ##" + aInput.first).c_str()))
				aInput.second.buttonType = InputActionButtonType::Repeated;

			if (ImGui::Selectable(("Down ##" + aInput.first).c_str()))
				aInput.second.buttonType = InputActionButtonType::Down;

			if (ImGui::Selectable(("Up ##" + aInput.first).c_str()))
				aInput.second.buttonType = InputActionButtonType::Up;

			ImGui::EndPopup();
		}
	}

	std::string GetButtonName(int aKey)
	{
		if (aKey >= 65 && aKey <= 91)
		{
			std::string keyNameString(1, static_cast<char>(aKey));
			return keyNameString;
		}

		switch (static_cast<Keycode::Scancode>(aKey))
		{
		case Keycode::ESCAPE: return "Escape";

		case Keycode::PLUS: return "+";

		case Keycode::N0: return "0";
		case Keycode::N1: return "1";
		case Keycode::N2: return "2";
		case Keycode::N3: return "3";
		case Keycode::N4: return "4";
		case Keycode::N5: return "5";
		case Keycode::N6: return "6";
		case Keycode::N7: return "7";
		case Keycode::N8: return "8";
		case Keycode::N9: return "9";

		case Keycode::ACUTE_ACCENT: return "´";
		case Keycode::SECTION_SIGN: return "§";
		case Keycode::LESS_THAN: return "<";
		case Keycode::UMLAUT: return "¨";

		case Keycode::BACKSPACE: return "Backspace";

		case Keycode::INSERT: return "Insert";
		case Keycode::HOME: return "Home";
		case Keycode::PG_UP: return "PageUp";
		case Keycode::DELETE_KEY: return "Delete";
		case Keycode::END: return "End";
		case Keycode::PG_DOWN: return "PageDown";

		case Keycode::LEFT: return "Arrow Left";
		case Keycode::UP: return "Arrow Up";
		case Keycode::RIGHT: return "Arrow Right";
		case Keycode::DOWN: return "Arrow Down";

		case Keycode::TAB: return "Tab";
		case Keycode::CAPS_LOCK: return "CapsLock";

		case Keycode::L_SHIFT: return "L-Shift";
		case Keycode::R_SHIFT: return "R-Shift";

		case Keycode::L_CTRL: return "L-Ctrl";
		case Keycode::R_CTRL: return "R-Ctrl";

		case Keycode::L_WINDOWS: return "L-Windows";
		case Keycode::R_WINDOWS: return "R-Windows";

		case Keycode::L_ALT: return "L-Alt";
		case Keycode::R_ALT: return "R-Alt";

		case Keycode::ENTER: return "Enter";

		case Keycode::SPACE: return "Space";

		case Keycode::F1: return "F1";
		case Keycode::F2: return "F2";
		case Keycode::F3: return "F3";
		case Keycode::F4: return "F4";
		case Keycode::F5: return "F5";
		case Keycode::F6: return "F6";
		case Keycode::F7: return "F7";
		case Keycode::F8: return "F8";
		case Keycode::F9: return "F9";
		case Keycode::F10: return "F10";
		case Keycode::F11: return "F11";
		case Keycode::F12: return "F12";

		case Keycode::AA: return "Å";
		case Keycode::AAA: return "Ä";
		case Keycode::OO: return "Ö";

		case Keycode::COMMA: return ",";
		case Keycode::DOT: return ".";

		case Keycode::ABSTERIX: return "*";
		}

		return "None";
	}
	 
	void ChangeButton(Keycode::Scancode& aKeycode)
	{
		ImGui::Text("Press Any Key");
		if (Input::GetAny())
		{
			aKeycode = Input::GetAnyKey();
			ImGui::CloseCurrentPopup();
		}


		ImGui::EndPopup();
	}

	void ButtonSelect(std::pair<const std::string, InputAction>& aInput)
	{
		if (aInput.second.type == InputActionType::Axis)
		{
			std::string buttonNamePos = GetButtonName(static_cast<int>(aInput.second.positiveButton)) + "##" + aInput.first;
			std::string buttonNameNeg = GetButtonName(static_cast<int>(aInput.second.negativeButton)) + "##" + aInput.first;

			ImGui::Text("Positive Button: ");
			ImGui::SameLine();
			if (ImGui::Button(buttonNamePos.c_str())) ImGui::OpenPopup(("PressKeyPopupPos" + aInput.first).c_str());

			ImGui::Text("Negative Button: ");
			ImGui::SameLine();
			if (ImGui::Button(buttonNameNeg.c_str())) ImGui::OpenPopup(("PressKeyPopupNeg" + aInput.first).c_str());
		}
		else if (aInput.second.type == InputActionType::Single)
		{
			std::string buttonName = GetButtonName(static_cast<int>(aInput.second.button)) + "##" + aInput.first;

			ImGui::Text("Button: ");
			ImGui::SameLine();
			if (ImGui::Button(buttonName.c_str())) ImGui::OpenPopup(("PressKeyPopup" + aInput.first).c_str());
		}
		else if (aInput.second.type == InputActionType::Combo)
		{
			auto& comboButtons = aInput.second.comboButtons;

			ImGui::Text("Combo Buttons: ");

			for (int i = 0; i < comboButtons.size(); i++)
			{
				std::stringstream stringstream;
				stringstream << i;

				std::string buttonName = GetButtonName(static_cast<int>(comboButtons[i]));
				buttonName.append("##");
				buttonName.append(aInput.first);
				buttonName.append(stringstream.str());

				std::string pressKeyCombo = "PressKeyPopup" + buttonName;

				if (ImGui::Button(buttonName.c_str()))
				{
					ImGui::OpenPopup(pressKeyCombo.c_str());
				}
				ImGui::SameLine();

				if (ImGui::Button(("-## " + pressKeyCombo).c_str()))
				{
					if (i != comboButtons.size() - 1)
						for (int j = i; j < comboButtons.size() - 1; j++)
							comboButtons[j] = comboButtons[j + 1];

					comboButtons.pop_back();
				}

				if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
					ImGui::SetTooltip("Removes the button\nto the left");

				if (i != comboButtons.size() - 1)
				{
					ImGui::Text("+");
				}
			}

			if (ImGui::Button(("Add" + aInput.first).c_str()))
				comboButtons.emplace_back(Keycode::NONE);

		}

		if (ImGui::BeginPopup(("PressKeyPopupPos" + aInput.first).c_str()))
			ChangeButton(aInput.second.positiveButton);
		if (ImGui::BeginPopup(("PressKeyPopupNeg" + aInput.first).c_str()))
			ChangeButton(aInput.second.negativeButton);
		if (ImGui::BeginPopup(("PressKeyPopup" + aInput.first).c_str()))
			ChangeButton(aInput.second.button);

		for (int i = 0; i < aInput.second.comboButtons.size(); i++)
		{
			auto& button = aInput.second.comboButtons[i];

			std::stringstream stringstream;
			stringstream << i;

			std::string buttonName = GetButtonName(static_cast<int>(button)) + "##" + aInput.first + stringstream.str();

			std::string pressKeyCombo = "PressKeyPopup" + buttonName;

			if (ImGui::BeginPopup(pressKeyCombo.c_str()))
				ChangeButton(button);
		}
	}

	void InputEditorWindow::Update()
	{
		bool refreshInputsButton = ImGui::Button("Refresh");
		if (refreshInputsButton)
			InputMapper::Load();

		ImGui::SameLine();

		bool saveButton = ImGui::Button("Save");
		if (saveButton)
			InputMapper::Save();

		std::vector<inputNameChange> inputsToChange;
		std::vector<std::string> inputsToRemove;

		auto& inputMap = InputMapper::GetInputMap();
		for (auto& input : inputMap)
		{
			if (ImGui::Button(("-##" + input.first).c_str()))
				inputsToRemove.emplace_back(input.first);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip(("Removes  [" + input.first + "]  input action").c_str());

			ImGui::SameLine();

			ImGuiStyleVar flags = 12;
			ImGui::PushStyleVar(flags, 0.f);
			ImGui::SetNextItemWidth(10.f);
			bool notCollapsed = ImGui::CollapsingHeader((input.first + "## ButtonName").c_str());

			ImGui::PopStyleVar();

			if (notCollapsed)
			{
				ImGui::Text("Action Name: ");
				ImGui::SameLine();

				char TemporaryName[256];
				std::strcpy(TemporaryName, input.first.c_str());

				bool nameChange = ImGui::InputText(("##ActionNameChange" + input.first).c_str(), TemporaryName, 256, ImGuiInputTextFlags_EnterReturnsTrue);
				if (nameChange)
					if (strcmp(input.first.c_str(), TemporaryName))
						inputsToChange.emplace_back(inputNameChange(input.first, TemporaryName, input.second));

				ActionType(input);
				ButtonType(input);

				ButtonSelect(input);
			}
		}

		for (auto& inputChange : inputsToChange)
		{
			inputMap.erase(inputChange.inputNamePrev);
			inputChange.inputAction.name = inputChange.inputNameNew;
			inputMap.emplace(inputChange.inputNameNew, inputChange.inputAction);
		}
		for (auto& inputRemove : inputsToRemove)
		{
			inputMap.erase(inputRemove);
		}

		ImGui::Dummy({ 0, 0 });
		ImGui::Dummy({ 0, 0 });

		if (ImGui::Button("  New  "))
		{
			inputMap["NewInput"].name = "NewInput";
			inputMap["NewInput"].button = Keycode::NONE;
			inputMap["NewInput"].buttonType = InputActionButtonType::Down;
			inputMap["NewInput"].type = InputActionType::Single;
		}

	}
}