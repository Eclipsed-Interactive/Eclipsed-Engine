#pragma once

#include <string>
#include "CoreEngine/Macros/defines.h"

#include <unordered_map>
#include <functional>
#include <typeindex>

#include "ImGui/imgui.h"

namespace Eclipse { class Component; }
namespace Eclipse::Reflection
{
	class ECLIPSED_API AbstractSerializedVariable
	{
		friend class ReflectionManager;

	public:

		virtual ~AbstractSerializedVariable();

		//#ifdef ECLIPSED_EDITOR
		AbstractSerializedVariable(const char* aName, Component* aCompPtr, bool drawInspector, float aChangeAmount);
		virtual void DrawInspector() = 0;
		//#else
		//		AbstractSerializedVariable(const char* aName, Component* aCompPtr);
		//#endif

	public:
		virtual void* GetRawData() = 0;
		virtual void* GetData() = 0;
		virtual const void* GetData()const = 0;
		virtual void ResolveTypeInfo() = 0;


		virtual void Resize(const size_t& size) = 0;


	public:
		static inline unsigned i = 0;
		
		static std::unordered_map<std::type_index, ImGuiDataType> types;
		static std::unordered_map<ImGuiDataType, size_t> sizes;

#ifdef ECLIPSED_EDITOR

		void DrawTest();
#endif

		enum SerializedTypes_
		{
			SerializedType_None,

			SerializedType_String,

			SerializedType_Material,
			SerializedType_AudioClip,
			SerializedType_Texture,
			SerializedType_Prefab,
			SerializedType_Font,

			SerializedType_Enum,
			SerializedType_Custom_Type,
			SerializedType_Bool,
			SerializedType_Color,
			SerializedType_Fundamental,

			SerializedType_List,
			SerializedType_Array,

			SerializedType_Component,
			SerializedType_Gameobject,
		};

	public:
		const char* GetName() const;
		Component* GetComponent();
		const Component* GetComponent() const;
		unsigned GetSizePerElement() const;
		unsigned GetSizeInBytes() const;
		unsigned GetCount() const;
		SerializedTypes_ GetType() const;
		virtual ImGuiDataType GetImGuiType() const = 0;

		bool IsValid() const;

		bool hasData = false;

		unsigned GetReplicationID()
		{
			return ReplicatedVariableIndex;
		}
		unsigned ReplicatedVariableIndex = 0;
	protected:

		const char* name = "";
		Component* pComponent = nullptr;
		bool componentHasDrawInspector = false;

		SerializedTypes_ type = SerializedType_None;
		unsigned sizePerElement = 0;
		unsigned count = 1;

		//#ifdef ECLIPSED_EDITOR
		std::string GetNameID() const;

		const char* nameID = "";
		bool canDrawInspector = false;

		float myChangAmount;

		unsigned imguiID;
	};
}