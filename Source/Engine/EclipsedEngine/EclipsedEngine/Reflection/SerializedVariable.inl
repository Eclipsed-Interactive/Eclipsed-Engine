#include "SerializedVariable.h"

#include "ReflectionTypeChecks.h"
#include "CoreEngine/SerializedEnum.hpp"
#include "ImGui/imgui.h"

#include "CoreEngine/Math/Math.h"

#include <typeindex>

//#include "AssetEngine/Assets/Material.h"
//#include "AssetEngine/Assets/AudioClip.h"
//#include "AssetEngine/Assets/Font.h"
//#include "AssetEngine/Assets/Prefab.h"
#include "AssetEngine/Assets/Texture/TextureAsset.h"

#include "CoreEngine/Math/Random.h"

namespace Eclipse::Reflection
{

	//#ifdef ECLIPSED_EDITOR

	template<typename T>
	inline SerializedVariable<T>::SerializedVariable(const char* aName, Component* aCompPtr, bool drawInspector, float aChangeAmount)
		: AbstractSerializedVariable(aName, aCompPtr, drawInspector, aChangeAmount)
	{

	}

	template<typename T>
	inline SerializedVariable<T>::SerializedVariable(const char* aName, Component* aCompPtr, bool drawInspector, float aChangeAmount, T _min, T _max)
		: AbstractSerializedVariable(aName, aCompPtr, drawInspector, aChangeAmount), myMin(_min), myMax(_max), hasMinMax(true)
	{

	}

	template<typename T>
	inline SerializedVariable<T>::SerializedVariable(const char* aName, Component* aCompPtr, bool drawInspector, float aChangeAmount, const T& aDefaultValue)
		: AbstractSerializedVariable(aName, aCompPtr, drawInspector, aChangeAmount), data(aDefaultValue)
	{

	}

	template<typename T>
	inline SerializedVariable<T>::SerializedVariable(const char* aName, Component* aCompPtr, bool drawInspector, float aChangeAmount, const T& aDefaultValue, T _min, T _max)
		: AbstractSerializedVariable(aName, aCompPtr, drawInspector, aChangeAmount), data(aDefaultValue), myMin(_min), myMax(_max), hasMinMax(true)
	{

	}

	template<typename T>
	inline void SerializedVariable<T>::DrawInspector()
	{
		ImGui::Text(GetName());
		ImGui::SameLine();
	}
	//#else
	//	template<typename T>
	//	inline SerializedVariable<T>::SerializedVariable(const char* aName, Component* aCompPtr)
	//		: AbstractSerializedVariable(aName, aCompPtr)
	//	{
	//	}
	//
	//	template<typename T>
	//	inline SerializedVariable<T>::SerializedVariable(const char* aName, Component* aCompPtr, const T& aDefaultValue)
	//		: AbstractSerializedVariable(aName, aCompPtr), data(aDefaultValue)
	//	{
	//	}
	//#endif

	template<typename T>
	concept HasGetType = requires(T type) { type.GetType(); };

	template<typename T>
	inline int SerializedVariable<T>::GetImGuiType() const
	{
		if (types.find(typeid(T)) != types.end())
		{
			return types.at(typeid(T));
		}

		if constexpr (HasGetType<T>)
		{
			const auto& tadwad = data.GetType();
			return types.at(tadwad);
		}

		return 20000;
	}

	inline bool AbstractSerializedVariable::IsValid() const
	{
		return hasData;
	}

	template<typename T>
	inline T* SerializedVariable<T>::GetHandle()
	{
		return &data;
	}

	template<typename T>
	inline void* SerializedVariable<T>::GetRawData()
	{
		return &data;
	}

	template<typename T>
	inline void* SerializedVariable<T>::GetData()
	{
		if constexpr (Is_Array<T>::value)
			return &data[0];

		else if constexpr (Is_Vector<T>::value)
			return &data[0];

		else
			return &data;
	}

	template<typename T>
	inline const void* SerializedVariable<T>::GetData() const
	{
		if constexpr (Is_Array<T>::value)
			return &data[0];

		else if constexpr (Is_Vector<T>::value)
			return &data[0];

		else
			return &data;
	}

	template<typename T>
	inline void SerializedVariable<T>::ResolveTypeInfo()
	{
		if constexpr (Is_String<T>::value)
		{
			type = SerializedType_String;
			sizePerElement = sizeof(char);
			if constexpr (Is_C_String<T>::value) count = std::strlen(data);
			else if constexpr (Is_Std_String<T>::value) count = data.size();
		}

		else if constexpr (Is_Serializable<T>::value)
		{
			type = SerializedType_Custom_Type;
			sizePerElement = sizeof(T);
		}

		//else if constexpr (std::is_same<T, Assets::Material>::value)
		//{
		//	type = SerializedType_Material;
		//	sizePerElement = sizeof(Assets::Material);
		//}
		//else if constexpr (std::is_same<T, Assets::AudioClip>::value)
		//{
		//	type = SerializedType_AudioClip;
		//	sizePerElement = sizeof(Assets::AudioClip);
		//}
		else if constexpr (std::is_same<T, Assets::Texture>::value)
		{
			type = SerializedType_Texture;
			sizePerElement = sizeof(Assets::Texture);
		}
		//else if constexpr (std::is_same<T, Assets::Prefab>::value)
		//{
		//	type = SerializedType_Prefab;
		//	sizePerElement = sizeof(Assets::Prefab);
		//}
		//else if constexpr (std::is_same<T, Assets::Font>::value)
		//{
		//	type = SerializedType_Font;
		//	sizePerElement = sizeof(Assets::Font);
		//}

		else if constexpr (std::is_same<T, Math::Color>::value)
		{
			type = SerializedType_Color;
			sizePerElement = sizeof(T);
		}
		
		else if constexpr (std::is_same<T, bool>::value)
		{
			type = SerializedType_Bool;
			sizePerElement = sizeof(T);
		}
		
		else if constexpr (std::is_arithmetic<T>::value)
		{
			type = SerializedType_Fundamental;
			sizePerElement = sizeof(T);
		}

		else if constexpr (Is_Array<T>::value)
		{
			type = SerializedType_Array;
			if constexpr (Is_C_Array<T>::value) count = 0; // C_ArrayCount(data);
			else if constexpr (Is_Std_Array<T>::value) count = data.size();

			sizePerElement = sizeof(data[0]);
		}

		else if constexpr (Is_Vector<T>::value)
		{
			type = SerializedType_List;
			count = data.size();
			sizePerElement = sizeof(data[0]);
		}

		else if constexpr (std::is_base_of<SerializedEnum, T>::value)
		{
			type = SerializedType_Enum;
			sizePerElement = sizeof(T::e);
		}

		else
		{

		}
	}

	template<typename T>
	inline void SerializedVariable<T>::Resize(const size_t& size)
	{
		if constexpr (Is_Vector<T>::value) data.resize(size);
	}


	//#ifdef ECLIPSED_EDITOR
	template<typename T>
	bool ComboEnum(const char* label, T& e) {
		unsigned currentIndex = static_cast<unsigned>(e);
		bool changed = false;

		if (ImGui::BeginCombo(label, T::AsString(T(currentIndex)).c_str()))
		{
			for (const T& val : T::List)
			{
				bool isSelected = (currentIndex == val);
				std::string name = T::AsString(val);

				if (ImGui::Selectable(name.c_str(), isSelected)) {
					e = static_cast<T>(val);
					changed = true;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		return changed;
	}

	//template<typename T>
	//inline void SerializedVariable<T>::DrawInspector()
	//{
	//	//EditorReflectionDrawHelper::DrawReflectedVariable(*this);
	//	if constexpr (Is_Vector<T>::value || Is_Array<T>::value)
	//	{
	//		isDrawn = ImGui::CollapsingHeader(GetName());

	//		if (!isDrawn) return;

	//		ResolveTypeInfo();
	//		for (int i = 0; i < GetCount(); ++i)
	//		{
	//			ImGui::Dummy({ 20, 0 });
	//			ImGui::SameLine();

	//			ImGui::PushID(i);

	//			if constexpr (Is_Vector<T>::value)
	//			{
	//				ImGui::SameLine();

	//				if (ImGui::Button("-"))
	//				{
	//					data.erase(data.begin() + i);
	//					ImGui::PopID();
	//					return;
	//				}

	//				ImGui::SameLine();
	//			}

	//			DrawElement(data[i]);
	//			ImGui::PopID();
	//		}

	//		if constexpr (Is_Vector<T>::value)
	//		{
	//			ImGui::Dummy({ 20, 0 });
	//			ImGui::SameLine();

	//			if (ImGui::Button("Add"))
	//			{
	//				data.push_back({});
	//			}
	//		}
	//	}
	//	else
	//	{
	//		auto name = GetName();

	//		ImGui::Text(name);
	//		ImGui::SameLine();
	//		DrawElement(data);
	//	}
	//}

	template<typename T>
	template<typename U>
	inline void SerializedVariable<T>::DrawElement(U& element)
	{
		float availX = ImGui::GetContentRegionAvail().x;

		ImGui::SetNextItemWidth(availX);

		//if constexpr (std::is_same<U, float>::value) Editor::EditorField::Draw(element);
		if constexpr (std::is_same<U, float>::value) ImGui::DragFloat(GetNameID().c_str(), &element, 0.001f, hasMinMax ? myMin : 0.f, hasMinMax ? myMax : 0.f);
		else if constexpr (std::is_same<U, int>::value) ImGui::DragInt(GetNameID().c_str(), &element, 1.f, hasMinMax ? myMin : 0, hasMinMax ? myMax : 0);
		else if constexpr (std::is_same<U, Math::Vector2<float>>::value) ImGui::DragFloat2(GetNameID().c_str(), &element.x, 0.001f);
		else if constexpr (std::is_same<U, bool>::value) ImGui::Checkbox(GetNameID().c_str(), &element);
		else if constexpr (Is_String<U>::value) DrawString(data);
		else if constexpr (std::is_base_of<SerializedEnum, U>::value) ComboEnum(GetNameID().c_str(), element);

		else ImGui::Text("Type: %s is not supported", typeid(T).name());
	}

	template<typename T>
	template<typename U>
	inline void SerializedVariable<T>::DrawString(U& element)
	{
		char TemporaryName[256];
		std::strcpy(TemporaryName, element.c_str());

		if (ImGui::InputText(GetName(), TemporaryName, 256, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			element = TemporaryName;
		}
	}
	//#endif
}