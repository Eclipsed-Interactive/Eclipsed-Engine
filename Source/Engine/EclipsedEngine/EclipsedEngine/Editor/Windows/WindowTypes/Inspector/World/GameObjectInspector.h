#pragma once

#include "EclipsedEngine/Editor/Windows/WindowTypes/Inspector/ITypeInspector.h"
#include <unordered_map>
#include <array>

#define NAME_BUFFER_LENGTH 256

namespace Eclipse::Editor 
{
	class GameObjectInspector 
	{
	public:
		static void Draw(const GameObjectTarget& target);

	private:
		static void DrawComponents(unsigned int gobjId);

		static char* GetNameBuffer(unsigned int id, const std::string& initialName = "Gameobject");
		static std::unordered_map<unsigned int, std::array<char, NAME_BUFFER_LENGTH>>& GetNameBuffers();
	};
}