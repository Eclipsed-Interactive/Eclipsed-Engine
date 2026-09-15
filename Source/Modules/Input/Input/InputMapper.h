#pragma once

#include <unordered_map>
#include <string_view>
#include <string>

#include "Keycodes.h"

#include "Input.Core.hpp"

namespace Eclipse::Input
{
	struct INPUT_API Axis
	{
		std::string Name;
		unsigned long long ID;
		Keycode::Scancode Positive;
		Keycode::Scancode Negative;
	};

	struct INPUT_API Action
	{
		std::string Name;
		unsigned long long ID;
		std::vector<Keycode::Scancode> Keys;
	};

	enum class INPUT_API ActionState
	{
		WasPressed,
		IsPressed,
		WasReleased
	};

	class INPUT_API InputMapper
	{
	public:
		static InputMapper* CreateMapping(std::string_view mapName);

		static std::unordered_map<std::string, InputMapper>& GetMappings();
		static InputMapper* GetMapping(std::string_view mapName);

	public:
		unsigned long long CreateAxis(std::string_view axisName, Keycode::Scancode positive, Keycode::Scancode negative);
		unsigned long long CreateAction(std::string_view actionName, Keycode::Scancode key = Keycode::Scancode::UNKNOWN);
		void Bind(std::string_view actionName, Keycode::Scancode key);

		bool WasPressed(std::string_view actionName);
		bool IsPressed(std::string_view actionName);
		bool WasReleased(std::string_view actionName);

		int GetAxis(std::string_view axisName);

	public:
		std::unordered_map<unsigned long long, Axis>& GetAxises();
		std::unordered_map<unsigned long long, Action>& GetActions();

		std::unordered_map<std::string, unsigned long long>& GetAxisNames();
		std::unordered_map<std::string, unsigned long long>& GetActionNames();

	public:
		Axis& FindAxis(std::string_view axisName);
		Action& FindAction(std::string_view actionName);

	public:
		void Save();
		static InputMapper* Load(std::string_view path);

	private:
		bool IsActionPressed(const Action& actionName, ActionState state = ActionState::IsPressed);

	private:
		std::unordered_map<unsigned long long, Axis> registeredAxis;
		std::unordered_map<std::string, unsigned long long> axisNames;

		std::unordered_map<unsigned long long, Action> registeredActions;
		std::unordered_map<std::string, unsigned long long> actionNames;

		std::string path;

		static std::unordered_map<std::string, InputMapper> mappings;
	};
}