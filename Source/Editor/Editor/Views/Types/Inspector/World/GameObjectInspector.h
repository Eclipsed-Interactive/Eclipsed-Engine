#pragma once

#include "Editor/Views/Types/Inspector/ITypeInspector.h"
#include <unordered_map>
#include <array>

#include "ImGui/imgui.h"
#include "ECS/Types.h"

#define NAME_BUFFER_LENGTH 256

namespace Eclipse::Editor 
{
	class GameObjectInspector 
	{
	public:
		static void Draw(const GameObjectTarget& target);

	private:
		static void DrawComponents(GameObjectID gobjId);

		static char* GetNameBuffer(GameObjectID id, const std::string& initialName = "Gameobject");
		static std::unordered_map<GameObjectID, std::array<char, NAME_BUFFER_LENGTH>, GUIDHash>& GetNameBuffers();

	private:
		static ImGuiTextFilter filter;
		static bool compSearchIsFocused;
	};
}