#ifdef ECL_EDITOR

#include "GameObjectInspector.h"

#include "ImGui/ImGui.h"
#include <string>
#include <unordered_map>
#include"../InspectableTarget.h"

#include "ECS/ComponentManager.h"

#include "EclipsedEngine/Reflection/Reflection.h"
#include "EclipsedEngine/Reflection/Registry/ComponentRegistry.h"

#include "Editor/ImGui/ImGui_Impl.h"

#include "Editor/Views/Types/Inspector/InspectorFieldDrawer.h"

namespace Eclipse::Editor
{
	ImGuiTextFilter GameObjectInspector::filter{};
	bool GameObjectInspector::compSearchIsFocused = false;

	void GameObjectInspector::Draw(const GameObjectTarget& target)
	{
		if (target == 0)
			return;

		if (ComponentManager::myEntityIdToEntity.find(target) == ComponentManager::myEntityIdToEntity.end())
			return;

		GameObject* gameObject = ComponentManager::myEntityIdToEntity.at(target);
		if (gameObject == nullptr) return;

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
		int counter = 0;

		static Component* rightClickedComp = nullptr;
		for (auto comp : ComponentManager::GetComponents(gobjId))
		{
			if (!comp)
				continue;

			std::string id = comp->GetComponentName();
			id += "##" + std::to_string(counter);

			counter++;

			ImGui_Impl::DrawComponentHeader(id.c_str(), comp->myInspectorWasDrawn);

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseReleased(1))
				{
					ImGui::OpenPopup("InspectorRightClickedComponent");
					rightClickedComp = comp;
				}
			}

			if (!comp->myInspectorWasDrawn) continue;


			ImGui::Indent(20.f);
			ImGui::Text("Is Replicated");
			ImGui::SameLine();
			ImGui::Checkbox(("##IsReplicatedBool" + id).c_str(), &comp->IsReplicated);

			const Reflection::TypeEntry* type = Reflection::ReflectionRegistry::TryGetType(comp->GetComponentName());
			if (!type)
			{
				ImGui::TextDisabled("Not Reflected");
			}
			else
			{
				ImGui::PushID(comp);
				for (const auto& [name, field] : type->fields)
				{
					ImGui::PushID(name.c_str());
					ImGui::Text(name.c_str());
					ImGui::SameLine();
					InspectorFieldDrawer::Draw((void*)comp, field);
					ImGui::PopID();
				}
				ImGui::PopID();
			}

			ImGui::Unindent(20.f);

			ImGui::Dummy({ 0, 30 });
			ImGui::Separator();
		}

		if (ImGui::BeginCombo("##ADD_COMPONENTS", "Add Component"))
		{
			if (!compSearchIsFocused)
			{
				ImGui::SetKeyboardFocusHere();
				filter = {};
				compSearchIsFocused = true;
			}

			filter.Draw("##Search");

			for (auto& [name, type] : Reflection::ReflectionRegistry::GetTypes())
			{
				if (type.flag != Reflection::ReflectionFlags::Component)
					continue;

				if (ImGui::Button(name.c_str(), ImVec2(-FLT_MIN, 0)))
				{
					ComponentManager::AddComponent(gobjId, type.createAtAddress, type.size);
					ImGui::CloseCurrentPopup();
					filter = {};
					compSearchIsFocused = false;
				}
			}

			//for (auto& [name, registeredComp] : ComponentRegistry::GetComponentTypeNameMap())
			//{
			//	if (!filter.PassFilter(name.c_str())) continue;

			//	if (ImGui::Button(name.c_str(), ImVec2(-FLT_MIN, 0)))
			//	{
			//		registeredComp.addComp(gobjId);
			//		ImGui::CloseCurrentPopup();
			//		filter = {};
			//		compSearchIsFocused = false;
			//	}
			//}

			ImGui::EndCombo();
		}
		else if (compSearchIsFocused)
		{
			filter = {};
			compSearchIsFocused = false;
		}

		if (ImGui::BeginPopup("InspectorRightClickedComponent"))
		{
			if (ImGui::Button("Remove"))
			{
				ComponentManager::DeleteComponent(gobjId,0, rightClickedComp->GetComponentID());
				//Destroy(rightClickedComp);
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

#endif