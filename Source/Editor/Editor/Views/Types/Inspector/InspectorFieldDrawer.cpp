#include "InspectorFieldDrawer.h"

#include "Reflection/Attributes/AttributeContainer.h"
#include "Reflection/Attributes/Attributes.hpp"
#include "AssetSystem/AssetTypes.h"

#include "Core/Math/Random.h"

#include "ImGui/imgui.h"

#include <Windows.h>
#include <string>
#include <algorithm>
#include <filesystem>
#include <vector>
#include <cfloat>

#include "Core/MainSingleton.h"
#include "Core/PathManager.h"

#include "Editor/Common/DragAndDrop.h"

#include "Reflection/Reflection.hpp"
#include "Reflection/TypeRegistry.h"

#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_stdlib.h"

#include <FontAwesome/7/IconsFontAwesome7.h>

#include <AssetSystem/Assets/Types/AudioClip.h>
#include <AssetSystem/Assets/Types/Material.h>
#include <AssetSystem/Assets/Types/Texture.h>

namespace Eclipse::Editor
{
	namespace
	{
		void InvokeOnChange(void* anObject, const Reflection::FieldEntry& anEntry)
		{
			if (const Reflection::OnChange* onChange = anEntry.attributeContainer.TryGet<Reflection::OnChange>())
			{
				if (onChange->event)
					onChange->event(anObject);
			}
		}

		bool RoundedButton()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			const ImGuiStyle& style = ImGui::GetStyle();

			ImVec2 labelSize = ImGui::CalcTextSize(ICON_FA_XMARK, nullptr, true);

			float width = labelSize.x + style.FramePadding.x * 2.f;
			float height = labelSize.y + style.FramePadding.y * 2.f;

			const ImVec2 min = ImGui::GetCursorScreenPos();
			const ImVec2 max = ImVec2(min.x + width, min.y + height);
			const ImVec2 size = ImVec2(width, width);

			auto color = ImGui::GetColorU32(ImGuiCol_FrameBgHovered);

			ImGui::InvisibleButton("##", size);
			if (ImGui::IsItemHovered())
			{
				color = ImGui::GetColorU32(ImGuiCol_FrameBgActive);
			}

			{
				const ImVec2 textSize = ImGui::CalcTextSize(ICON_FA_XMARK);
				const ImVec2 textPos = {
					min.x + (size.x - textSize.x) * 0.5f,
					min.y + size.y - textSize.y
				};


				ImDrawFlags flags = ImDrawFlags_RoundCornersRight;


				drawList->AddRectFilled(min, max, color, style.FrameRounding, flags);
				drawList->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), ICON_FA_XMARK);
			}



			return false;
		}

		template<typename T>
		bool DrawAssetField(T* asset, const char* aDnd)
		{
			bool changed = false;
			GUID guid = asset->GetGUID();
			ImGui::PushID(&guid);

			const bool hasAsset = !guid.IsEmpty();

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			const ImGuiStyle& style = ImGui::GetStyle();

			float height = ImGui::GetFrameHeight();
			ImVec2 btnSize = ImVec2(height, height);

			ImVec2 start = ImGui::GetCursorScreenPos();
			float rightEdge = start.x + ImGui::GetContentRegionAvail().x;

			ImVec2 fieldMin = start;
			ImVec2 fieldMax = ImVec2(rightEdge - btnSize.x, start.y + height);
			ImVec2 fieldSize = ImVec2(fieldMax.x - fieldMin.x, height);

			{ // Field 
				std::string label = hasAsset ? guid.ToString() : "No asset.";

				const bool clicked = ImGui::InvisibleButton("##field", fieldSize);

				auto color = ImGui::GetColorU32(ImGuiCol_FrameBg);

				if (ImGui::IsItemActive()) color = ImGui::GetColorU32(ImGuiCol_FrameBgActive);
				else if (ImGui::IsItemHovered()) color = ImGui::GetColorU32(ImGuiCol_FrameBgHovered);

				drawList->AddRectFilled(fieldMin, fieldMax, color, style.FrameRounding, ImDrawFlags_RoundCornersLeft);
				ImGui::RenderTextClipped(ImVec2(fieldMin.x + style.FramePadding.x, fieldMin.y), 
					ImVec2(fieldMax.x - style.FramePadding.x, fieldMax.y),
					label.c_str(), nullptr, nullptr, ImVec2(0.f, 0.5f));

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(aDnd))
					{
						const std::string guidStr(static_cast<const char*>(payload->Data), payload->DataSize);
						GUID newGuid;
						newGuid.FromString(guidStr);

						if (!newGuid.IsEmpty())
						{
							*asset = Assets::Load<T>(newGuid);
							changed = true;
						}
					}
					ImGui::EndDragDropTarget();
				}
			}

			ImVec2 btnMin = ImVec2(rightEdge - btnSize.x, start.y);
			ImVec2 btnMax = ImVec2(rightEdge, start.y + height);
			ImGui::SetCursorScreenPos(btnMin);

			{ // Clear button
				const bool clicked = ImGui::InvisibleButton("##clear", btnSize);
				
				auto color = ImGui::GetColorU32(ImGuiCol_Button);

				if(ImGui::IsItemActive()) color = ImGui::GetColorU32(ImGuiCol_ButtonActive);
				else if (ImGui::IsItemHovered()) color = ImGui::GetColorU32(ImGuiCol_ButtonHovered);

				drawList->AddRectFilled(btnMin, btnMax, color, style.FrameRounding, ImDrawFlags_RoundCornersRight);
				ImGui::RenderTextClipped(btnMin, btnMax, ICON_FA_XMARK, nullptr, nullptr, ImVec2(0.5f, 0.5f));

				if (clicked)
				{
					*asset = {};
					changed = true;
				}
			}

			ImGui::PopID();

			return changed;
		}
	}

	void InspectorFieldDrawer::Draw(void* v, const Reflection::FieldEntry& field)
	{
		unsigned char* object = reinterpret_cast<unsigned char*>(v) + field.offset;

		if (DrawValue(object, field.type, field.attributeContainer))
		{
			InvokeOnChange(v, field);
			return;
		}

	}







	void InspectorFieldDrawer::RegisterDefaults()
	{
		using namespace Reflection;

		// register all of the numeric types
		RegisterNumeric<
			char, signed char, unsigned char,
			short, unsigned short,
			int, unsigned,
			long, unsigned long,
			long long, unsigned long long,
			float, double
		>();

		Register<Math::Vector2<bool>>([](void* v, const Reflection::AttributeContainer& attrs) -> bool {

			ImGui::Text("X");
			ImGui::SameLine();
			bool changed = ImGui::Checkbox("##valuex", reinterpret_cast<bool*>(v));

			ImGui::SameLine();

			ImGui::Text("Y");
			ImGui::SameLine();
			changed |= ImGui::Checkbox("##valuey", reinterpret_cast<bool*>(v) + sizeof(bool));

			return changed;
			}
		);

		Register<bool>([](void* v, const Reflection::AttributeContainer& attrs) -> bool {
			return ImGui::Checkbox("##value", reinterpret_cast<bool*>(v));
			}
		);

		Register<std::string>([](void* v, const Reflection::AttributeContainer& attrs) {
			return ImGui::InputText("##value", reinterpret_cast<std::string*>(v));
			}
		);

		Register<Math::Color>([](void* v, const Reflection::AttributeContainer& attrs) {
			return ImGui::ColorEdit4("##value", &reinterpret_cast<Math::Color*>(v)->r);
			}
		);


		Register<AudioClip>([](void* v, const Reflection::AttributeContainer& attrs) {
			return DrawAssetField(reinterpret_cast<AudioClip*>(v), DragAndDrop::dnd_id[Utilities::FileInfo::FileType_Audio]);
			}
		);

		Register<Texture>([](void* v, const Reflection::AttributeContainer& attrs) {
			return DrawAssetField(reinterpret_cast<Texture*>(v), DragAndDrop::dnd_id[Utilities::FileInfo::FileType_Texture]);
			}
		);

		Register<Material>([](void* v, const Reflection::AttributeContainer& attrs) {
			return DrawAssetField(reinterpret_cast<Material*>(v), DragAndDrop::dnd_id[Utilities::FileInfo::FileType_Material]);
			}
		);
	}

	bool InspectorFieldDrawer::DrawValue(void* value, TypeID type, const Reflection::AttributeContainer& attrs)
	{
		if (auto it = drawInspectorFuncs.find(type);  it != drawInspectorFuncs.end())
			return it->second(value, attrs);

		if (const Reflection::ContainerOps* ops = Reflection::ReflectionRegistry::TryGetContainer(type))
			return DrawContainer(value, *ops, attrs);

		if (const auto* reflected = Reflection::ReflectionRegistry::TryGetType(type); reflected != nullptr)
		{
			bool wasChanged = false;
			if (ImGui::BeginTable("##fields", 2, ImGuiTableFlags_Resizable))
			{
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 100);
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

				for (const std::string& fieldName : reflected->fieldOrder)
				{
					const Reflection::FieldEntry& field = Reflection::ReflectionRegistry::GetField(type, fieldName);

					ImGui::PushID(fieldName.c_str());
					ImGui::TableNextRow();

					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted(fieldName.c_str()); // Format the name

					ImGui::TableNextColumn();
					ImGui::PushItemWidth(-FLT_MIN);

					void* fieldPtr = static_cast<unsigned char*>(value) + field.offset;
					wasChanged |= InspectorFieldDrawer::DrawValue(fieldPtr, field.type, field.attributeContainer);
					ImGui::PopItemWidth();
					ImGui::PopID();
				}
				ImGui::EndTable();

				ImGui::Separator();
			}

			return wasChanged;
		}

		NoDrawMethod();
		return false;
	}

	void InspectorFieldDrawer::NoDrawMethod()
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.1f, 0.1f, 1.f));
		ImGui::Text(__FILE__);
		ImGui::Text("No draw method.");
		ImGui::PopStyleColor();
	}



	bool InspectorFieldDrawer::DrawContainer(void* container, const Reflection::ContainerOps& ops, const Reflection::AttributeContainer& attrs)
	{
		size_t size = ops.size(container);

		const ImGuiStyle& style = ImGui::GetStyle();

		const float rightEdge = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x;
		const float btn = ImGui::GetFrameHeight();
		const float btnPos = rightEdge - btn * 2 - ImGui::GetStyle().ItemInnerSpacing.x;

		const float sizeDragWidth = 50.f;

		const float controlsWidth = style.ItemSpacing.x
			+ sizeDragWidth + style.ItemSpacing.x
			+ btn + style.ItemSpacing.x
			+ btn;

		const float btnWidth = btn * 2 + style.ItemInnerSpacing.x;

		const std::string sizeStr = std::to_string(size);
		const std::string header = size > 0 ? (size == 1 ? sizeStr + " element" : sizeStr + " element") : "Empty";

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		const float oldWorkMaxX = window->WorkRect.Max.x;
		window->WorkRect.Max.x -= controlsWidth + style.ItemSpacing.x;
		bool open = ImGui::TreeNodeEx("##elementsofthevector", ImGuiTreeNodeFlags_AllowOverlap, header.c_str());
		window->WorkRect.Max.x = oldWorkMaxX;


		bool wasChanged = false;

		ImGui::SameLine();
		ImGui::SetCursorPosX(rightEdge - controlsWidth);
		ImGui::SetNextItemWidth(sizeDragWidth);

		if (ImGui::DragScalar("##size", ImGuiDataType_U64, &size, 1, nullptr, nullptr, nullptr, ImGuiSliderFlags_AlwaysClamp))
		{
			ops.resize(container, size);
			wasChanged = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("+", ImVec2(btn, btn)))
		{
			ops.resize(container, size + 1);
			wasChanged = true;
		}

		ImGui::SameLine();

		ImGui::SetNextItemWidth(ImGui::GetFrameHeight());

		ImGui::BeginDisabled(size == 0);
		if (ImGui::Button("-", ImVec2(btn, btn)))
		{
			ops.resize(container, size - 1);
			wasChanged = true;
		}
		ImGui::EndDisabled();

		if (open)
		{
			for (size_t i = 0; i < ops.size(container); i++)
			{
				ImGui::Text(("Element " + std::to_string(i + 1)).c_str());

				ImGui::SameLine();

				ImGui::PushID(i);
				wasChanged |= DrawValue(ops.at(container, i), ops.elementType, attrs);
				ImGui::PopID();
			}

			ImGui::TreePop();
		}

		return wasChanged;
	}
}