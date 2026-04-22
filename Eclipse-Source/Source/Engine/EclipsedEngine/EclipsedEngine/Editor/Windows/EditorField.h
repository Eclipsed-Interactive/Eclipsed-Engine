#pragma once

#include <vector>
#include <type_traits>
#include <typeinfo>
#include "ImGui/imgui.h"

namespace Eclipse::Editor
{
	template<typename>
	inline constexpr bool always_false_v = false;

	template<typename T>
	struct EditorFieldDrawer {
		static bool Draw(T&) {
			static_assert(always_false_v<T>, R"(The implementation dont exists, please view the output window.
The error you want to keep an eye out for look like this:
	[
		T = Eclipse::Math::Color
	])");
		}
	};

	template<typename T>
	inline ImGuiDataType_ GetImGuiType()
	{
		using Clean = std::remove_pointer_t<std::remove_cvref_t<T>>;

		if constexpr (std::is_same_v<Clean, char>) return ImGuiDataType_S8;
		else if constexpr (std::is_same_v<Clean, unsigned char>) return ImGuiDataType_U8;
		else if constexpr (std::is_same_v<Clean, short>) return ImGuiDataType_S16;
		else if constexpr (std::is_same_v<Clean, unsigned short>) return ImGuiDataType_U16;
		else if constexpr (std::is_same_v<Clean, int>) return ImGuiDataType_S32;
		else if constexpr (std::is_same_v<Clean, unsigned int>) return ImGuiDataType_U32;
		else if constexpr (std::is_same_v<Clean, long long>) return ImGuiDataType_S64;
		else if constexpr (std::is_same_v<Clean, unsigned long long>) return ImGuiDataType_U64;
		else if constexpr (std::is_same_v<Clean, float>) return ImGuiDataType_Float;
		else if constexpr (std::is_same_v<Clean, double>) return ImGuiDataType_Double;
		else if constexpr (std::is_same_v<Clean, bool>) return ImGuiDataType_Bool;
		else if constexpr (std::is_same_v<Clean, std::string>) return ImGuiDataType_String;
		else static_assert("Type not supported.");
	}

	inline std::string GetFieldKey()
	{
		return  "##" + std::to_string(rand());
	}

	template<typename T>
	inline bool DrawEditorField(T& aValue)
	{
		//using Clean = std::remove_pointer_t<std::remove_cvref_t<T>>;
		return EditorFieldDrawer<T>::Draw(aValue);
	}
}

#include "EditorFields.inl"