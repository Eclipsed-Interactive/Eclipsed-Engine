#include "AbstractSerializedVariable.h"

#include "Reflection.h"

//#include "Editor/Common/DragAndDrop.h"
#include "Assets/Assets/MaterialAsset.h"
#include "Assets/Assets/AudioAsset.h"
#include "Assets/Assets/TextureAsset.h"
#include "Assets/Assets/FontAsset.h"
#include "Assets/Assets/PrefabAsset.h"
#include "Assets/AssetManager.h"

#include "ReflectionTypeChecks.h"
#include "ECS/Component.h"

#include "Core/MainSingleton.h"
#include "Assets/Core/AssetDatabase.h"

//#include "Editor/Windows/EditorField.h"

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

	//#ifdef ECL_EDITOR
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

#ifdef ECL_EDITOR
	void AbstractSerializedVariable::DrawTest()
	{
		//ResolveTypeInfo();
		//ImGuiDataType iType = GetImGuiType();

		////bool drawDefualt = pComponent->DrawDefaultInspector();

		//if (/*drawDefault*/true)
		//{
		//	int size = 0;
		//	int elements = 1;
		//	//if(iType == ImGuiDataType_String) size = 


		//	size = sizes[iType];
		//	if (type == SerializedType_Custom_Type) elements = sizePerElement / size;

		//	ImVec2 startPos = ImGui::GetCursorPos();
		//	ImGui::Text(GetName());

		//	switch (type)
		//	{
		//	case SerializedType_Material:
		//	{
		//		ImGui::SameLine();

		//		std::string name = "No material.";

		//		Assets::Material* mat = (Assets::Material*)GetData();
		//		if (mat->IsValid())
		//		{
		//			name = MainSingleton::GetInstance<Assets::AssetDatabase>().GetProcessedFile(mat->GetAssetID()).fileName;
		//		}

		//		if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Material))
		//		{
		//			*mat = Assets::AssetManager::Load<Assets::Material>(MainSingleton::GetInstance<Assets::AssetDatabase>().GetProcessedFile(Editor::DragAndDrop::payloadBuffer).guid);

		//			mat->Create();
		//		}
		//	} break;


		//	case SerializedType_AudioClip:
		//	{
		//		ImGui::SameLine();

		//		std::string name = "No audio clip.";

		//		Assets::AudioClip* clip = (Assets::AudioClip*)GetData();
		//		if (clip->IsValid())
		//		{
		//			name = MainSingleton::GetInstance<Assets::AssetDatabase>().GetProcessedFile(clip->GetAssetID()).fileName;
		//		}

		//		if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Audio))
		//		{
		//			Assets::GUID guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetProcessedFile(Editor::DragAndDrop::payloadBuffer).guid;
		//			Assets::AudioClip clips = Assets::AssetManager::Load<Assets::AudioClip>(guid);
		//			
		//			*clip = clips;
		//		}
		//	} break;

		//	case SerializedType_Texture:
		//	{
		//		ImGui::SameLine();

		//		std::string name = "No texture.";

		//		Assets::Texture* texture = (Assets::Texture*)GetData();
		//		if (texture->IsValid())
		//		{
		//			name = MainSingleton::GetInstance<Assets::AssetDatabase>().GetProcessedFile(texture->GetAssetID()).fileName;
		//		}

		//		if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Texture))
		//		{
		//			Assets::GUID guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetProcessedFile(Editor::DragAndDrop::payloadBuffer).guid;
		//			Assets::Texture _texture = Assets::AssetManager::Load<Assets::Texture>(guid);
		//			*texture= _texture;
		//		}
		//	} break;

		//	case SerializedType_Prefab:
		//	{
		//		ImGui::SameLine();

		//		std::string name = "No Prefab.";

		//		Assets::Prefab* prefab = (Assets::Prefab*)GetData();
		//		if (prefab->IsValid())
		//		{
		//			name = MainSingleton::GetInstance<Assets::AssetDatabase>().GetProcessedFile(prefab->GetAssetID()).fileName;
		//		}

		//		if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Prefab))
		//		{
		//			Assets::GUID guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetProcessedFile(Editor::DragAndDrop::payloadBuffer).guid;
		//			Assets::Prefab _texture = Assets::AssetManager::Load<Assets::Prefab>(guid);
		//			*prefab = _texture;
		//		}
		//	} break;

		//	case SerializedType_Font:
		//		{
		//			ImGui::SameLine();

		//			std::string name = "No Font.";

		//			Assets::Font* font = (Assets::Font*)GetData();
		//			if (font->IsValid())
		//			{
		//				name = MainSingleton::GetInstance<Assets::AssetDatabase>().GetProcessedFile(font->GetAssetID()).fileName;
		//			}

		//			if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Font))
		//			{
		//				Assets::GUID guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetProcessedFile(Editor::DragAndDrop::payloadBuffer).guid;
		//				Assets::Font _texture = Assets::AssetManager::Load<Assets::Font>(guid);
		//				*font = _texture;
		//			}
		//		} break;

		//	case SerializedType_List:
		//	case SerializedType_Array:
		//	case SerializedType_Component:
		//	case SerializedType_Gameobject:
		//	case SerializedType_Enum:
		//		break;



		//	case SerializedType_String:
		//	{
		//		ImGui::SameLine();

		//		char TemporaryName[256];
		//		std::string* data = reinterpret_cast<std::string*>(GetData());
		//		std::strcpy(TemporaryName, data->c_str());

		//		if (ImGui::InputText(("##" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), TemporaryName, 256, ImGuiInputTextFlags_EnterReturnsTrue))
		//		{
		//			size_t size = strlen(TemporaryName);
		//			data->resize(size);
		//			memcpy(data->data(), &TemporaryName, size + 1);
		//		}
		//	}
		//		break;
		//	case SerializedType_Bool:
		//		ImGui::SameLine();
		//		ImGui::Checkbox(("##" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), (bool*)GetData());
		//		break;
		//	case SerializedType_Color:
		//		{
		//			ImGui::SameLine();
		//			bool ColorbuttonClick = ImGui::ColorButton(("##ColorButton" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), *(ImVec4*)GetData());
		//			if (ColorbuttonClick)
		//				ImGui::OpenPopup("InspectorColorPickerPopUp");
		//			if (ImGui::BeginPopup("InspectorColorPickerPopUp"))
		//			{
		//				ImGui::ColorPicker4(("##" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), (float*)GetData());
		//				ImGui::EndPopup();
		//			}
		//		}
		//		break;
		//	case SerializedType_Custom_Type:
		//	case SerializedType_Fundamental:

		//		ImGui::SameLine();
		//		ImGui::DragScalarN(("##" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), iType, GetData(), elements, myChangAmount);
		//		break;
		//	}

		//	//ImGui::SetCursorPosX(startPos.x);
		//}

		//pComponent->OnDrawInspector();
		//if (pComponent->HasDrawInspector())
		//{
		//}
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