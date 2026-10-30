#include "InspectorFieldDrawer.h"

#include "Reflection/Attributes/AttributeContainer.h"
#include "Reflection/Attributes/Attributes.hpp"

#include "Core/Math/Random.h"

#include "ImGui/imgui.h"

#include <Windows.h>
#include <string>
#include <algorithm>
#include <filesystem>

#include "Core/Math/Vector/Vector2.h"

#include "Assets/Assets/AudioAsset.h"
#include "Assets/AssetManager.h"
#include "Assets/Core/AssetDatabase.h"

#include "Core/MainSingleton.h"
#include "Core/PathManager.h"

#include "Editor/Common/DragAndDrop.h"

namespace Eclipse::Editor
{
	namespace
	{
		// Asset browser payloads are paths relative to the tree they came from (project or engine assets).
		bool TryGetGUIDFromPayload(const ImGuiPayload* payload, Assets::GUID& outGuid)
		{
			const std::string relativePath(static_cast<const char*>(payload->Data), payload->DataSize);
			Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();

			for (const std::filesystem::path& root : { PathManager::GetAssetsPath(), PathManager::GetEngineAssetsPath() })
			{
				try
				{
					outGuid = database.GetGUIDFromFullPath(root / relativePath);
					return true;
				}
				catch (const std::runtime_error&) {}
			}

			return false;
		}

		template<typename TAsset>
		std::string GetAssetDisplayName(const TAsset& asset, const char* typeName)
		{
			if (!asset.IsValid()) return std::string("None (") + typeName + ")";

			try
			{
				const Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();
				return database.GetProcessedFile(asset.GetAssetID()).fileName;
			}
			catch (const std::runtime_error&)
			{
				return std::string("Missing (") + typeName + ")";
			}
		}

		// Unity-style object field: a box showing the asset name that accepts drops from the asset browser.
		template<typename TAsset>
		void DrawAssetField(TAsset& asset, Utilities::FileInfo::FileType_ fileType, const char* typeName)
		{
			const char* dndId = DragAndDrop::dnd_id[fileType];
			const std::string label = GetAssetDisplayName(asset, typeName);

			const ImGuiStyle& style = ImGui::GetStyle();
			const float clearWidth = ImGui::GetFrameHeight();
			const float boxWidth = (std::max)(50.f, ImGui::GetContentRegionAvail().x - clearWidth - style.ItemInnerSpacing.x);
			const ImVec2 boxSize(boxWidth, ImGui::GetFrameHeight());

			const ImVec2 boxMin = ImGui::GetCursorScreenPos();
			const ImVec2 boxMax(boxMin.x + boxSize.x, boxMin.y + boxSize.y);

			ImGui::InvisibleButton("##AssetField", boxSize);
			const bool hovered = ImGui::IsItemHovered();

			// Only highlight when the thing being dragged is actually something we accept.
			const ImGuiPayload* dragged = ImGui::GetDragDropPayload();
			const bool compatibleDrag = dragged && dragged->IsDataType(dndId);

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(boxMin, boxMax, ImGui::GetColorU32(hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), style.FrameRounding);
			if (compatibleDrag)
				drawList->AddRect(boxMin, boxMax, ImGui::GetColorU32(ImGuiCol_DragDropTarget), style.FrameRounding, 0, 1.5f);

			const ImVec4 textColor = asset.IsValid() ? style.Colors[ImGuiCol_Text] : style.Colors[ImGuiCol_TextDisabled];
			const ImVec2 textPos(boxMin.x + style.FramePadding.x, boxMin.y + style.FramePadding.y);
			drawList->PushClipRect(boxMin, ImVec2(boxMax.x - style.FramePadding.x, boxMax.y), true);
			drawList->AddText(textPos, ImGui::GetColorU32(textColor), label.c_str());
			drawList->PopClipRect();

			if (hovered && asset.IsValid())
				ImGui::SetTooltip("%s", asset.GetAssetID().ToString().c_str());

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dndId))
				{
					Assets::GUID guid;
					if (TryGetGUIDFromPayload(payload, guid))
						asset = Assets::AssetManager::Load<TAsset>(guid);
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine(0.f, style.ItemInnerSpacing.x);
			ImGui::BeginDisabled(!asset.IsValid());
			if (ImGui::Button("X##ClearAsset", ImVec2(clearWidth, clearWidth)))
				asset = TAsset();
			ImGui::EndDisabled();
		}
	}

	void InspectorFieldDrawer::Draw(void* v, const Reflection::FieldEntry& field)
	{
		if (!drawInspectorFuncs.contains(field.type))
		{
			NoDrawMethod();
			return;
		}

		drawInspectorFuncs[field.type](v, field);
	}

	void InspectorFieldDrawer::RegisterDefaults()
	{
		using namespace Reflection;

		Register<int>([](void* v, const FieldEntry& entry) {
			const Range* range = entry.attributeContainer.TryGet<Range>();

			float step = 1.f;
			const Step* stepAttr = entry.attributeContainer.TryGet<Step>();
			if (stepAttr) step = stepAttr->step;

			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			if (range)
			{
				ImGui::DragInt("##value",
					(int*)addr,
					step,
					range->min,
					range->max
				);
			}
			else
			{
				ImGui::DragInt("##value", (int*)addr, step);
			}
			}
		);

		Register<float>([](void* v, const FieldEntry& entry) {
			const Range* range = entry.attributeContainer.TryGet<Range>();

			float step = 0.1f;
			const Step* stepAttr = entry.attributeContainer.TryGet<Step>();
			if (stepAttr) step = stepAttr->step;

			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			if (range)
			{
				ImGui::DragFloat("##value",
					(float*)addr,
					step,
					range->min,
					range->max
				);
			}
			else
			{
				ImGui::DragFloat("##value", (float*)addr, step);
			}
			}
		);

		Register<bool>([](void* v, const FieldEntry& entry) {
			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			ImGui::Checkbox("##value", (bool*)addr);
			}
		);

		Register<Math::Vector2f>([](void* v, const FieldEntry& entry) {
			const Range* range = entry.attributeContainer.TryGet<Range>();

			float step = 0.1f;
			const Step* stepAttr = entry.attributeContainer.TryGet<Step>();
			if (stepAttr) step = stepAttr->step;

			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			if (range)
			{
				ImGui::DragFloat2("##value",
					(float*)addr,
					step,
					range->min,
					range->max
				);
			}
			else
			{
				ImGui::DragFloat2("##value", (float*)addr, step);
			}
			}
		);

		Register<Math::Vector2f>([](void* v, const FieldEntry& entry) {
			const Range* range = entry.attributeContainer.TryGet<Range>();

			float step = 0.1f;
			const Step* stepAttr = entry.attributeContainer.TryGet<Step>();
			if (stepAttr) step = stepAttr->step;

			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			if (range)
			{
				ImGui::DragFloat2("##value",
					(float*)addr,
					step,
					range->min,
					range->max
				);
			}
			else
			{
				ImGui::DragFloat2("##value", (float*)addr, step);
			}
			}
		);

		Register<Assets::AudioClip>([](void* v, const FieldEntry& entry) {
			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			DrawAssetField(*reinterpret_cast<Assets::AudioClip*>(addr), Utilities::FileInfo::FileType_Audio, "Audio Clip");
			}
		);
	}

	void InspectorFieldDrawer::NoDrawMethod()
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.1f, 0.1f, 1.f));
		ImGui::Text(__FILE__);
		ImGui::Text("No draw method.");
		ImGui::PopStyleColor();
	}
}