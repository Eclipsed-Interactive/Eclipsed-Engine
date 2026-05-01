#include "GameObjectInspector.h"

#include "ImGui/ImGui.h"
#include <string>
#include <unordered_map>
#include"../InspectableTarget.h"

#include "EntityEngine/ComponentManager.h"

#include "EclipsedEngine/Editor/ImGui/ImGui_Impl.h"
#include "EclipsedEngine/Reflection/Reflection.h"
#include "EclipsedEngine/Reflection/Registry/ComponentRegistry.h"

#include "EclipsedEngine/ECS/ObjectManager.h"

namespace Eclipse::Editor
{
	void GameObjectInspector::Draw(const GameObjectTarget& target)
	{
		if (target == 0)
			return;

		GameObject* gameObject = ComponentManager::myEntityIdToEntity[target];

		ImGuiStyle& style = ImGui::GetStyle();
		const ImVec2 padding = style.WindowPadding;

		static bool enabled = false;
		ImGui::Checkbox("##game_object_enable", &enabled);

		ImGui::SameLine();

		char* name = GetNameBuffer(gameObject->GetID(), gameObject->GetName());
		if (ImGui::InputText("##gameobject_name", name, NAME_BUFFER_LENGTH,
			ImGuiInputTextFlags_EnterReturnsTrue))
		{
			gameObject->SetName(std::string(name));
		}

		ImGui::Spacing();
		ImGui::Separator();

		DrawComponents(gameObject->GetID());
	}

	void GameObjectInspector::DrawComponents(unsigned int gobjId)
	{
		static Component* rightClickedComp = nullptr;
		for (auto comp : ComponentManager::GetComponents(gobjId))
		{
			ImGui_Impl::DrawComponentHeader(comp->GetComponentName(), comp->myInspectorWasDrawn);

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseReleased(1))
				{
					ImGui::OpenPopup("InspectorRightClickedComponent");
					rightClickedComp = comp;
				}
			}

			if (!comp->myInspectorWasDrawn) continue;


			ImGui::Text("Is Replicated");
			ImGui::SameLine();
			ImGui::Checkbox(("##IsReplicatedBool" + std::to_string(gobjId)).c_str(), &comp->IsReplicated);

			ImGui::Indent(20.f);
			Reflection::ReflectionManager::DrawInspector(comp, comp->GetComponentName());
			ImGui::Unindent(20.f);

			ImGui::Dummy({ 0, 30 });
			ImGui::Separator();
		}

		if (ImGui::BeginCombo("##ADD_COMPONENTS", "Add Component"))
		{
			for (auto& [name, addFunc] : ComponentRegistry::GetInspectorAddComponentMap())
			{
				if (ImGui::Button(name.c_str(), ImVec2(-FLT_MIN, 0)))
				{
					addFunc(gobjId);
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndCombo();
		}

		if (ImGui::BeginPopup("InspectorRightClickedComponent"))
		{
			if (ImGui::Button("Remove"))
			{
				Destroy(rightClickedComp);
				rightClickedComp = nullptr;

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	char* GameObjectInspector::GetNameBuffer(unsigned int id, const std::string& initialName)
	{
		auto& nameBuffers = GetNameBuffers();
		auto [it, inserted] = nameBuffers.try_emplace(id);

		if (inserted)
		{
			strncpy(it->second.data(), initialName.c_str(), NAME_BUFFER_LENGTH - 1);
			it->second[NAME_BUFFER_LENGTH - 1] = '\0';
		}

		return it->second.data();
	}

	std::unordered_map<unsigned int, std::array<char, NAME_BUFFER_LENGTH>>& GameObjectInspector::GetNameBuffers()
	{
		static std::unordered_map<unsigned int, std::array<char, NAME_BUFFER_LENGTH>> nameBuffers;
		return nameBuffers;
	}
}