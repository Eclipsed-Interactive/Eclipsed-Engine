#include "SettingsView.h"

#include "Input/Input.h"
#include "Input/InputMapper.h"
#include "ImGui/imgui.h"

#include <algorithm>
#include <string>
#include <vector>

namespace Eclipse::Editor
{
	namespace
	{
		bool VectorContains(const std::vector<Keycode::Scancode>& values, Keycode::Scancode value)
		{
			return std::find(values.begin(), values.end(), value) != values.end();
		}
	}

	void SettingsView::OnOpen()
	{
		selectedMap = 0;
		actionSelection = 0;
		axisSelection = 0;

		mapName.clear();
		actionName.clear();

		type = SelectionType::Action;
		mapChanged = false;
	}

	void SettingsView::Draw()
	{
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Spacing();

			Indent(10);

			DrawInputMapper();

			ImGui::Spacing();

			DrawInputDebug();

			Unindent(10);
		}
	}

	void SettingsView::DrawInputDebug()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		if (!ImGui::CollapsingHeader("Debug"))
			return;

		ImVec2 start = ImGui::GetCursorScreenPos();

		Indent(10);

		ImGuiIO& io = ImGui::GetIO();

		ImGui::TextUnformatted("Window Mouse Position");
		ImGui::Separator();
		ImGui::Spacing();

		unsigned x = static_cast<unsigned>(io.MousePos.x);
		unsigned y = static_cast<unsigned>(io.MousePos.y);

		ImGui::Text("X");
		ImGui::SameLine(60.0f);
		ImGui::TextDisabled("%u", x);

		ImGui::Text("Y");
		ImGui::SameLine(60.0f);
		ImGui::TextDisabled("%u", y);

		ImGui::Dummy({ 0, 20 });

		ImGui::TextUnformatted("Game View Mouse Position");
		ImGui::Separator();
		ImGui::Spacing();

		Math::Vector2ui gameMouse = Input::Input::GetMousePos();

		ImGui::Text("X");
		ImGui::SameLine(60.0f);
		ImGui::TextDisabled("%u", gameMouse.x);

		ImGui::Text("Y");
		ImGui::SameLine(60.0f);
		ImGui::TextDisabled("%u", gameMouse.y);

		ImGui::Dummy({ 0, 20 });

		ImGui::TextUnformatted("Active Keys");
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::BeginChild("SettingsActiveKeys", ImVec2(ImGui::GetWindowSize().x - 20.0f, 70.0f), false, ImGuiWindowFlags_AlwaysHorizontalScrollbar);

		ImGui::Dummy({ 5, 0 });
		ImGui::SameLine();
		ImGui::Dummy({ 0, 10 });

		bool first = true;

		for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; key++)
		{
			ImGuiKey imguiKey = static_cast<ImGuiKey>(key);

			if (!ImGui::IsKeyDown(imguiKey))
				continue;

			if (first)
			{
				ImGui::Dummy({ 10, 0 });
				ImGui::SameLine();

				first = false;
			}

			ImGui::BeginGroup();

			ImVec2 pos = ImGui::GetCursorScreenPos();

			const char* name = ImGui::GetKeyName(imguiKey);

			if (!name || !name[0])
				name = "?";

			constexpr ImVec2 padding = { 24, 11 };

			ImVec2 textSize = ImGui::CalcTextSize(name);
			ImVec2 keySize(textSize.x + padding.x, textSize.y + padding.y);

			ImVec2 min = pos;
			ImVec2 max(pos.x + keySize.x, pos.y + keySize.y);

			ImGui::InvisibleButton("##active_key", keySize);

			drawList->AddRectFilled(min, max, IM_COL32(55, 55, 60, 255), 5.0f);
			drawList->AddRect(min, max, IM_COL32(100, 100, 110, 255), 5.0f);

			ImVec2 textPos(min.x + (keySize.x - textSize.x) * 0.5f, min.y + (keySize.y - textSize.y) * 0.5f);

			drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), name);

			ImGui::EndGroup();
			ImGui::SameLine(0.0f, 6.0f);
		}

		ImGui::EndChild();

		Unindent(10);

		ImVec2 end = ImGui::GetCursorScreenPos();

		end.x += 5.0f;

		drawList->AddRectFilled(start, end, IM_COL32(134, 109, 52, 255));

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
	}

	void SettingsView::DrawInputMapper()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		if (!ImGui::CollapsingHeader("Input Mapper"))
			return;

		ImVec2 start = ImGui::GetCursorScreenPos();

		Indent(10);

		auto& maps = Input::InputMapper::GetMappings();

		if (maps.empty())
		{
			ImGui::TextDisabled("No input maps.");

			ImGui::Spacing();

			if (ImGui::Button("Create Input Map"))
			{
				ImGui::OpenPopup("NewInputMapper");
			}

			DrawInputMapperPopups();

			Unindent(10);

			ImVec2 end = ImGui::GetCursorScreenPos();
			end.x += 5.0f;

			drawList->AddRectFilled(start, end, IM_COL32(134, 109, 52, 255));

			return;
		}

		std::vector<std::string> mapNames;

		for (auto& [name, map] : maps)
		{
			mapNames.push_back(name);
		}

		if (selectedMap < 0)
			selectedMap = 0;

		if (selectedMap >= static_cast<int>(mapNames.size()))
			selectedMap = static_cast<int>(mapNames.size()) - 1;

		if (mapName.empty())
		{
			mapName = mapNames[selectedMap];
		}

		std::vector<const char*> mapItems;

		for (const auto& name : mapNames)
		{
			mapItems.push_back(name.c_str());
		}

		if (ImGui::Combo("Input Map", &selectedMap, mapItems.data(), static_cast<int>(mapItems.size())))
		{
			mapName = mapNames[selectedMap];

			actionName.clear();

			actionSelection = 0;
			axisSelection = 0;

			type = SelectionType::Action;
		}

		auto map = Input::InputMapper::GetMapping(mapName);

		if (!map)
		{
			ImGui::TextDisabled("Invalid input map.");

			DrawInputMapperPopups();

			Unindent(10);

			return;
		}

		ImGui::Spacing();

		if (ImGui::Button("New Action"))
		{
			ImGui::OpenPopup("NewAction");
		}

		ImGui::SameLine();

		if (ImGui::Button("New Axis"))
		{
			ImGui::OpenPopup("NewAxis");
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::TextUnformatted("Actions");
		ImGui::Spacing();

		auto& actions = map->GetActions();

		if (actions.empty())
		{
			ImGui::TextDisabled("No actions.");
		}
		else
		{
			std::vector<std::string> actionNames;

			for (auto& [_, action] : actions)
			{
				actionNames.push_back(action.Name);
			}

			if (actionSelection < 0)
				actionSelection = 0;

			if (actionSelection >= static_cast<int>(actionNames.size()))
				actionSelection = static_cast<int>(actionNames.size()) - 1;

			std::vector<const char*> actionItems;

			for (const auto& name : actionNames)
			{
				actionItems.push_back(name.c_str());
			}

			if (ImGui::Combo("##ActionSelection", &actionSelection, actionItems.data(), static_cast<int>(actionItems.size())))
			{
				type = SelectionType::Action;

				actionName = actionNames[actionSelection];
			}

			if (actionName.empty() && !actionNames.empty())
			{
				actionName = actionNames[0];
			}

			ImGui::Spacing();

			if (!actionName.empty())
			{
				auto& action = map->FindAction(actionName);

				ImGui::Separator();

				ImGui::Text("Action: %s", action.Name.c_str());
				ImGui::Spacing();

				ImGui::Spacing();

				if (action.Keys.empty())
				{
					ImGui::TextDisabled("No keys assigned.");
				}
				else
				{
					if (ImGui::CollapsingHeader("Bindings"))
					{
						for (size_t i = 0; i < action.Keys.size(); ++i)
						{
							ImGui::PushID(static_cast<int>(i));

							ImGui::Bullet();

							ImGui::Text("%s", Keycode::ToString(action.Keys[i]).c_str());

							ImGui::SameLine();

							if (ImGui::SmallButton("Remove"))
							{
								action.Keys.erase(action.Keys.begin() + i);

								mapChanged = true;

								ImGui::PopID();

								break;
							}

							ImGui::PopID();
						}
					}
				}

				ImGui::Spacing();

				if (ImGui::Button("Add Key"))
				{
					ImGui::OpenPopup("AddActionKey");
				}

				ImGui::SameLine();

				if (ImGui::Button("Clear Keys"))
				{
					action.Keys.clear();

					mapChanged = true;
				}

				ImGui::Spacing();
			}
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::TextUnformatted("Axes");
		ImGui::Spacing();

		auto& axes = map->GetAxises();

		if (axes.empty())
		{
			ImGui::TextDisabled("No axes.");
		}
		else
		{
			std::vector<std::string> axisNames;

			for (auto& [_, axis] : axes)
			{
				axisNames.push_back(axis.Name);
			}

			if (axisSelection < 0)
				axisSelection = 0;

			if (axisSelection >= static_cast<int>(axisNames.size()))
				axisSelection = static_cast<int>(axisNames.size()) - 1;

			std::vector<const char*> axisItems;

			for (const auto& name : axisNames)
			{
				axisItems.push_back(name.c_str());
			}

			if (ImGui::Combo("##AxisSelection", &axisSelection, axisItems.data(), static_cast<int>(axisItems.size())))
			{
				type = SelectionType::Axis;

				axisName = axisNames[axisSelection];
			}

			if (axisName.empty() && !axisNames.empty())
			{
				axisName = axisNames[0];
			}

			ImGui::Spacing();

			if (!axisName.empty())
			{
				auto& axis = map->FindAxis(axisName);

				ImGui::Separator();

				ImGui::Text("Axis: %s", axis.Name.c_str());
				ImGui::Spacing();

				ImGui::Text("Negative:");
				ImGui::SameLine(100.0f);

				if (axis.Negative != Keycode::UNKNOWN)
					ImGui::TextDisabled("%s", Keycode::ToString(axis.Negative).c_str());
				else
					ImGui::TextDisabled("Not assigned");

				ImGui::SameLine();

				if (ImGui::SmallButton("Listen##AxisNegative"))
				{
					listeningAxisBinding = AxisBinding::Negative;
					ImGui::OpenPopup("ListenForAxisKey");
				}

				ImGui::Text("Positive:");
				ImGui::SameLine(100.0f);

				if (axis.Positive != Keycode::UNKNOWN)
					ImGui::TextDisabled("%s", Keycode::ToString(axis.Positive).c_str());
				else
					ImGui::TextDisabled("Not assigned");

				ImGui::SameLine();

				if (ImGui::SmallButton("Listen##AxisPositive"))
				{
					listeningAxisBinding = AxisBinding::Positive;
					ImGui::OpenPopup("ListenForAxisKey");
				}

				if (ImGui::BeginPopupModal("ListenForAxisKey", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::TextUnformatted("Press a key to bind it.");
					ImGui::Spacing();
					ImGui::TextDisabled("Press Stop to cancel.");

					Keycode::Scancode code = Input::Input::GetAnyKey();

					if (code != Keycode::UNKNOWN)
					{
						if (listeningAxisBinding == AxisBinding::Negative)
							axis.Negative = code;
						else
							axis.Positive = code;

						mapChanged = true;

						ImGui::CloseCurrentPopup();
					}

					ImGui::Spacing();

					if (ImGui::Button("Stop"))
					{
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

				ImGui::Spacing();
			}
		}

		if (ImGui::Button("Create new..."))
		{
			ImGui::OpenPopup("NewInputMapper");
		}

		ImGui::SameLine();

		ImGui::BeginDisabled(!mapChanged);

		if (ImGui::Button("Apply"))
		{
			for (auto& [name, map] : Input::InputMapper::GetMappings())
			{
				map.Save();
			}
		}

		ImGui::EndDisabled();

		DrawInputMapperPopups();

		Unindent(10);

		ImVec2 end = ImGui::GetCursorScreenPos();

		end.x += 5.0f;

		drawList->AddRectFilled(start, end, IM_COL32(134, 109, 52, 255));

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
	}

	void SettingsView::DrawInputMapperPopups()
	{
		static char input[256] = "";
		if (ImGui::BeginPopupModal("NewInputMapper", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::TextUnformatted("Name of map");
			ImGui::Spacing();

			ImGui::InputText("##newinputmap", input, sizeof(input));
			ImGui::Spacing();

			if (ImGui::Button("Create"))
			{
				if (input[0] != '\0')
				{
					Input::InputMapper::CreateMapping(input);

					mapName = input;
					input[0] = '\0';
					mapChanged = true;

					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
			{
				input[0] = '\0';

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("NewAction", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::TextUnformatted("Name of Action");
			ImGui::Spacing();

			ImGui::InputText("##newActionName", input, sizeof(input));
			ImGui::Spacing();

			if (ImGui::Button("Add Keys"))
			{
				ImGui::OpenPopup("Addkeysforactions");
			}

			ImGui::Spacing();

			static std::vector<Keycode::Scancode> newActionKeys;

			if (newActionKeys.empty())
			{
				ImGui::TextDisabled("No keys selected.");
			}
			else
			{
				ImGui::Text("Selected keys: %zu", newActionKeys.size());

				for (size_t i = 0; i < newActionKeys.size(); ++i)
				{
					ImGui::PushID(static_cast<int>(i));
					ImGui::Bullet();

					ImGui::Text("%s", Keycode::ToString(newActionKeys[i]).c_str());
					ImGui::SameLine();

					if (ImGui::SmallButton("Remove"))
					{
						newActionKeys.erase(newActionKeys.begin() + i);

						ImGui::PopID();

						break;
					}

					ImGui::PopID();
				}
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::Button("Create"))
			{
				if (input[0] != '\0')
				{
					auto map = Input::InputMapper::GetMapping(mapName);

					if (map)
					{
						map->CreateAction(input);

						Input::Action& action = map->FindAction(input);

						action.Keys.insert(action.Keys.end(), newActionKeys.begin(), newActionKeys.end());

						actionSelection = 0;

						actionName = input;

						newActionKeys.clear();

						input[0] = '\0';

						mapChanged = true;

						ImGui::CloseCurrentPopup();
					}
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
			{
				newActionKeys.clear();

				input[0] = '\0';

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::BeginPopupModal("Addkeysforactions", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::TextUnformatted("Press a key to add it.");
				ImGui::Spacing();

				ImGui::TextDisabled("Press Stop when finished.");
				ImGui::Spacing();


				if (ImGui::Button("Stop"))
				{
					ImGui::CloseCurrentPopup();
				}
				else if (!ImGui::IsItemHovered() && !Input::Input::GetMouse(Keycode::MOUSE_LEFT))
				{
					Keycode::Scancode code = Input::Input::GetAnyKey();
					if (code != Keycode::UNKNOWN)
					{
						if (!VectorContains(newActionKeys, code))
						{
							newActionKeys.push_back(code);
						}
					}
				}

				ImGui::EndPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("AddActionKey", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::TextUnformatted("Press a key to bind it.");
			ImGui::Spacing();
			ImGui::TextDisabled("Press Stop when finished.");

			auto map = Input::InputMapper::GetMapping(mapName);

			if (map && !actionName.empty())
			{
				auto& action = map->FindAction(actionName);

				Keycode::Scancode code = Input::Input::GetAnyKey();

				if (code != Keycode::UNKNOWN)
				{
					if (!VectorContains(action.Keys, code))
					{
						action.Keys.push_back(code);

						mapChanged = true;
					}
				}
			}

			ImGui::Spacing();

			if (ImGui::Button("Stop"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}



















		if (ImGui::BeginPopupModal("NewAxis", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::TextUnformatted("Name of Axis");
			ImGui::Spacing();

			ImGui::InputText("##newAxisName", input, sizeof(input));

			ImGui::Spacing();

			static Keycode::Scancode neg = Keycode::UNKNOWN;
			static Keycode::Scancode pos = Keycode::UNKNOWN;
			static int listening = 0; 

			ImGui::TextUnformatted("Negative:");
			ImGui::SameLine(100.0f);

			if (neg != Keycode::UNKNOWN)
				ImGui::Text("%s", Keycode::ToString(neg).c_str());
			else
				ImGui::TextDisabled("Not assigned");

			ImGui::SameLine();

			if (listening == 1)
			{
				if (ImGui::Button("Listening...##Negative"))
					listening = 0;
			}
			else
			{
				if (ImGui::Button("Listen##Negative"))
					listening = 1;
			}

			ImGui::TextUnformatted("Positive:");
			ImGui::SameLine(100.0f);

			if (pos != Keycode::UNKNOWN)
				ImGui::Text("%s", Keycode::ToString(pos).c_str());
			else
				ImGui::TextDisabled("Not assigned");

			ImGui::SameLine();

			if (listening == 2)
			{
				if (ImGui::Button("Listening...##Positive"))
					listening = 0;
			}
			else
			{
				if (ImGui::Button("Listen##Positive"))
					listening = 2;
			}

			ImGui::Spacing();

			if (listening != 0)
			{
				ImGui::TextDisabled("Press any key...");

				Keycode::Scancode code = Input::Input::GetAnyKey();

				if (code != Keycode::UNKNOWN)
				{
					if (listening == 1)
						neg = code;
					else if (listening == 2)
						pos = code;

					listening = 0;
				}
			}

			ImGui::Spacing();

			if (ImGui::Button("Create"))
			{
				if (input[0] && neg != Keycode::UNKNOWN && pos != Keycode::UNKNOWN)
				{
					const std::string axisName = input;

					auto map = Input::InputMapper::GetMapping(mapName);

					if (map)
					{
						map->CreateAxis(axisName, neg, pos);

						mapChanged = true;

						input[0] = '\0';
						neg = Keycode::UNKNOWN;
						pos = Keycode::UNKNOWN;
						listening = 0;

						ImGui::CloseCurrentPopup();
					}
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
			{
				input[0] = '\0';
				neg = Keycode::UNKNOWN;
				pos = Keycode::UNKNOWN;
				listening = 0;

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void SettingsView::SaveMapper(const char* mapPath)
	{
		mapChanged = false;
	}

	void SettingsView::LoadMapper(const char* mapPath)
	{

	}
}