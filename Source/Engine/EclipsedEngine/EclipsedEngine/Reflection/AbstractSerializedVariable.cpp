#include "AbstractSerializedVariable.h"

#include "Reflection.h"

#include "EclipsedEngine/Editor/Common/DragAndDrop.h"
#include "AssetEngine/Assets/Material.h"
#include "AssetEngine/Assets/AudioClip.h"
#include "AssetEngine/Assets/Texture.h"
#include "AssetEngine/Assets/Prefab.h"
#include "AssetEngine/Resources.h"

#include "ReflectionTypeChecks.h"
#include "EntityEngine/Component.h"

#include "CoreEngine/MainSingleton.h"
#include "AssetEngine/AssetDatabase.h"

#include "EclipsedEngine/Editor/Windows/EditorField.h"

namespace Eclipse::Reflection
{
	std::unordered_map<std::type_index, ImGuiDataType> AbstractSerializedVariable::types = {
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
	
	std::unordered_map<ImGuiDataType, size_t> AbstractSerializedVariable::sizes = {
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

	
	AbstractSerializedVariable::~AbstractSerializedVariable()
	{
		ReflectionManager::UnregisterVariable(this);
	}

	//#ifdef ECLIPSED_EDITOR
	AbstractSerializedVariable::AbstractSerializedVariable(const char* aName, Component* aCompPtr, bool drawInspector, float aChangeAmount)
		: name(aName), pComponent(aCompPtr), canDrawInspector(drawInspector), myChangAmount(aChangeAmount)
	{
		ReflectionManager::RegisterVariable(this);
		imguiID = rand();
	}

	std::string AbstractSerializedVariable::GetNameID() const { return std::string("##") + std::string(name); }
	//#else 
	//	AbstractSerializedVariable::AbstractSerializedVariable(const char* aName, Component* aCompPtr)
	//		: name(aName), pComponent(aCompPtr)
	//	{
	//		ReflectionManager::RegisterVariable(this);
	//	}
	//#endif

#ifdef ECLIPSED_EDITOR
	void AbstractSerializedVariable::DrawTest()
	{
		ResolveTypeInfo();
		ImGuiDataType iType = GetImGuiType();

		//bool drawDefualt = pComponent->DrawDefaultInspector();

		if (/*drawDefault*/true)
		{
			int size = 0;
			int elements = 1;
			//if(iType == ImGuiDataType_String) size = 


			size = sizes[iType];
			if (type == SerializedType_Custom_Type) elements = sizePerElement / size;

			ImVec2 startPos = ImGui::GetCursorPos();
			ImGui::Text(GetName());

			switch (type)
			{
			case SerializedType_Material:
			{
				ImGui::SameLine();

				std::string name = "No material.";

				Material* mat = (Material*)GetData();
				if (mat->IsValid())
				{
					name = mat->GetAssetID();
				}

				if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Material))
				{
					size_t guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetMetaData(Editor::DragAndDrop::payloadBuffer).guid;
					*mat = Resources::Get<Material>(guid);

					mat->Create();
				}
			} break;


			case SerializedType_AudioClip:
			{
				ImGui::SameLine();

				std::string name = "No audio clip.";

				AudioClip* clip = (AudioClip*)GetData();
				if (clip->IsValid())
				{
					name = clip->GetAssetID();
				}

				if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Audio))
				{
					size_t guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetMetaData(Editor::DragAndDrop::payloadBuffer).guid;
					AudioClip clips = Resources::Get<AudioClip>(guid);
					*clip = clips;
				}
			} break;

			case SerializedType_Texture:
			{
				ImGui::SameLine();

				std::string name = "No texture.";

				Texture* texture = (Texture*)GetData();
				if (texture->IsValid())
				{
					name = texture->GetAssetID();
				}

				if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Texture))
				{
					size_t guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetMetaData(Editor::DragAndDrop::payloadBuffer).guid;
					*texture = Resources::Get<Texture>(guid);
				}
			} break;

			case SerializedType_Prefab:
			{
				ImGui::SameLine();

				std::string name = "No Prefab.";

				Prefab* prefab = (Prefab*)GetData();
				if (prefab->IsValid())
				{
					name = prefab->GetAssetID();
				}

				if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Prefab))
				{
					size_t guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetMetaData(Editor::DragAndDrop::payloadBuffer).guid;
					*prefab = Resources::Get<Prefab>(guid);
				}
			} break;

			case SerializedType_Font:
				{
					ImGui::SameLine();

					std::string name = "No Font.";

					Font* font = (Font*)GetData();
					if (font->IsValid())
					{
						name = font->GetAssetID();
					}

					if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Font))
					{
						size_t guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetMetaData(Editor::DragAndDrop::payloadBuffer).guid;
						*font = Resources::Get<Font>(guid);
					}
				} break;

			case SerializedType_List:
			case SerializedType_Array:
			case SerializedType_Component:
			case SerializedType_Gameobject:
			case SerializedType_Enum:
				break;



			case SerializedType_String:
			{
				ImGui::SameLine();

				char TemporaryName[256];
				std::string* data = reinterpret_cast<std::string*>(GetData());
				std::strcpy(TemporaryName, data->c_str());

				if (ImGui::InputText(("##" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), TemporaryName, 256, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					size_t size = strlen(TemporaryName);
					data->resize(size);
					memcpy(data->data(), &TemporaryName, size + 1);
				}
			}
				break;
			case SerializedType_Bool:
				ImGui::SameLine();
				ImGui::Checkbox(("##" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), (bool*)GetData());
				break;
			case SerializedType_Color:
				{
					ImGui::SameLine();
					bool ColorbuttonClick = ImGui::ColorButton(("##ColorButton" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), *(ImVec4*)GetData());
					if (ColorbuttonClick)
						ImGui::OpenPopup("InspectorColorPickerPopUp");
					if (ImGui::BeginPopup("InspectorColorPickerPopUp"))
					{
						ImGui::ColorPicker4(("##" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), (float*)GetData());
						ImGui::EndPopup();
					}
				}
				break;
			case SerializedType_Custom_Type:
			case SerializedType_Fundamental:

				ImGui::SameLine();
				ImGui::DragScalarN(("##" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), iType, GetData(), elements, myChangAmount);
				break;
			}

			//ImGui::SetCursorPosX(startPos.x);
		}

		pComponent->OnDrawInspector();
		if (pComponent->HasDrawInspector())
		{
		}
	}
#endif

	const char* AbstractSerializedVariable::GetName() const { return name; }
	Component* AbstractSerializedVariable::GetComponent() { return pComponent; }
	const Component* AbstractSerializedVariable::GetComponent() const { return pComponent; }
	unsigned AbstractSerializedVariable::GetSizePerElement() const { return sizePerElement; }
	unsigned AbstractSerializedVariable::GetSizeInBytes() const { return sizePerElement * count; }
	unsigned AbstractSerializedVariable::GetCount() const { return count; }
	AbstractSerializedVariable::SerializedTypes_ AbstractSerializedVariable::GetType() const { return type; }
}