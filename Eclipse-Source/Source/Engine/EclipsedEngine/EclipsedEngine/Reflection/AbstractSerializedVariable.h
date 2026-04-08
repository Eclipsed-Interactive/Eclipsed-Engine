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
		std::unordered_map<std::type_index, ImGuiDataType> types = {
			{typeid(char), ImGuiDataType_S8},
			{typeid(unsigned char), ImGuiDataType_U8},

			{typeid(short), ImGuiDataType_S16},
			{typeid(unsigned short), ImGuiDataType_U16},

			{typeid(int), ImGuiDataType_S32},
			{typeid(unsigned int), ImGuiDataType_U32},

			{typeid(long long), ImGuiDataType_S64},
			{typeid(unsigned long long), ImGuiDataType_U64},

			{typeid(float), ImGuiDataType_Float},
			{typeid(double), ImGuiDataType_Double},

			{typeid(bool), ImGuiDataType_Bool},

			{typeid(char*), ImGuiDataType_String},
			{typeid(const char*), ImGuiDataType_String},
			{typeid(std::string), ImGuiDataType_String},
		};


		std::unordered_map<ImGuiDataType, size_t> sizes = {
			{ImGuiDataType_S8, sizeof(char)},
			{ImGuiDataType_U8, sizeof(unsigned char)},

			{ImGuiDataType_S16, sizeof(short)},
			{ImGuiDataType_U16, sizeof(unsigned short)},

			{ImGuiDataType_S32, sizeof(int)},
			{ImGuiDataType_U32, sizeof(unsigned int)},

			{ImGuiDataType_S64, sizeof(long long)},
			{ImGuiDataType_U64, sizeof(unsigned long long)},

			{ImGuiDataType_Float, sizeof(float)},
			{ImGuiDataType_Double, sizeof(double)},

			{ImGuiDataType_Bool, sizeof(bool)},

			{ImGuiDataType_String, 20000},
		};

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