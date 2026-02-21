#include "AbstractSerializedVariable.h"

#include "Reflection.h"

#include "EclipsedEngine/Editor/Common/DragAndDrop.h"
#include "AssetEngine/Assets/Material.h"
#include "AssetEngine/Assets/AudioClip.h"
#include "AssetEngine/Assets/Texture.h"
#include "AssetEngine/Assets/Prefab.h"
#include "AssetEngine/Resources.h"

#include "AssetEngine/Editor/MetaFile/MetaFileRegistry.h"
#include "ReflectionTypeChecks.h"
#include "EntityEngine/Component.h"


#include "EclipsedEngine/Editor/Windows/EditorField.h"

namespace Eclipse::Reflection
{
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
					std::string guid = MetaFileRegistry::GetGUID(Editor::DragAndDrop::payloadBuffer);
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
					std::string guid = MetaFileRegistry::GetGUID(Editor::DragAndDrop::payloadBuffer);
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
					std::string guid = MetaFileRegistry::GetGUID(Editor::DragAndDrop::payloadBuffer);
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
					std::string guid = MetaFileRegistry::GetGUID(Editor::DragAndDrop::payloadBuffer);
					*prefab = Resources::Get<Prefab>(guid);
				}
			} break;


			case SerializedType_List:
			case SerializedType_Array:
			case SerializedType_Component:
			case SerializedType_Gameobject:
			case SerializedType_Enum:
				break;



			case SerializedType_String:
				ImGui::SameLine();

				char TemporaryName[256];
				std::strcpy(TemporaryName, static_cast<const char*>(GetData()));

				if (ImGui::InputText(("##" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), TemporaryName, 256, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					memcpy(GetData(), &TemporaryName, strlen(TemporaryName) + 1);
				}
				break;
			case SerializedType_Bool:
				ImGui::SameLine();
				ImGui::Checkbox(("##" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), (bool*)GetData());
				break;

			case SerializedType_Custom_Type:
			case SerializedType_Fundamental:

				ImGui::SameLine();
				ImGui::DragScalarN(("##" + std::to_string(imguiID) + std::to_string(iType) + GetName()).c_str(), iType, GetData(), elements, myChangAmount);
				break;
			}

			ImGui::SetCursorPosX(startPos.x);
		}

		pComponent->OnDrawInspector();
		if (pComponent->HasDrawInspector())
		{
		}
	}

	const char* AbstractSerializedVariable::GetName() const { return name; }
	Component* AbstractSerializedVariable::GetComponent() { return pComponent; }
	const Component* AbstractSerializedVariable::GetComponent() const { return pComponent; }
	unsigned AbstractSerializedVariable::GetSizePerElement() const { return sizePerElement; }
	unsigned AbstractSerializedVariable::GetSizeInBytes() const { return sizePerElement * count; }
	unsigned AbstractSerializedVariable::GetCount() const { return count; }
	AbstractSerializedVariable::SerializedTypes_ AbstractSerializedVariable::GetType() const { return type; }
}