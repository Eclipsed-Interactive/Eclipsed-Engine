#include "InspectorFieldDrawer.h"

#include "Reflection/Attributes/AttributeContainer.h"
#include "Reflection/Attributes/Attributes.hpp"

#include "Core/Math/Random.h"

#include "ImGui/imgui.h"

#include <Windows.h>
#include <string>
#include <algorithm>
#include <filesystem>
#include <vector>
#include <cfloat>

#include "Core/Math/Vector/Vector2.h"

#include "Assets/Assets/AudioAsset.h"
#include "Assets/Assets/TextureAsset.h"
#include "Assets/Assets/MaterialAsset.h"

#include "Assets/AssetManager.h"
#include "Assets/Core/AssetDatabase.h"

#include "Core/MainSingleton.h"
#include "Core/PathManager.h"

#include "Editor/Common/DragAndDrop.h"

namespace Eclipse::Editor
{
	namespace
	{
		bool TryGetGUIDFromPayload(const ImGuiPayload* aPayload, Assets::GUID& aOutGuid)
		{
			const std::string relativePath(static_cast<const char*>(aPayload->Data), aPayload->DataSize);
			Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();

			for (const std::filesystem::path& root : { PathManager::GetAssetsPath(), PathManager::GetEngineAssetsPath() })
			{
				try
				{
					aOutGuid = database.GetGUIDFromFullPath(root / relativePath);
					return true;
				}
				catch (...)
				{

				}
			}

			return false;
		}

		template<typename TAsset>
		std::string GetDisplayName(const TAsset& asset, const char* typeName)
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

		bool DrawAssetPicker(const char* popupId, Assets::AssetType assetType, const Assets::GUID& current, Assets::GUID& outGuid)
		{
			if (!ImGui::BeginPopup(popupId)) return false;

			static ImGuiTextFilter filter;
			if (ImGui::IsWindowAppearing())
			{
				filter.Clear();
				ImGui::SetKeyboardFocusHere();
			}
			filter.Draw("##Filter", -FLT_MIN);

			std::vector<const Assets::AssetMeta*> assets;
			for (const auto& [guid, meta] : MainSingleton::GetInstance<Assets::AssetDatabase>().GetSources())
			{
				if (meta.type == assetType && filter.PassFilter(meta.fileName.c_str()))
					assets.push_back(&meta);
			}
			std::sort(assets.begin(), assets.end(), [](const Assets::AssetMeta* a, const Assets::AssetMeta* b) { return a->fileName < b->fileName; });

			bool picked = false;
			if (ImGui::BeginListBox("##assets", ImVec2(-FLT_MIN, 10.f * ImGui::GetTextLineHeightWithSpacing())))
			{
				if (ImGui::Selectable("None", current.IsEmpty()))
				{
					outGuid = Assets::GUID();
					picked = true;
				}

				for (const Assets::AssetMeta* meta : assets)
				{
					ImGui::PushID(meta);
					if (ImGui::Selectable(meta->fileName.c_str(), meta->guid == current))
					{
						outGuid = meta->guid;
						picked = true;
					}
					if (ImGui::IsItemHovered() && !meta->fullPath.empty())
						ImGui::SetTooltip("%s", meta->fullPath.generic_string().c_str());
					ImGui::PopID();
				}
				ImGui::EndListBox();
			}

			ImGui::EndPopup();
			return picked;
		}


		template<typename TAsset>
		void DrawAssetField(TAsset& anAsset, Utilities::FileInfo::FileType_ aFileType, Assets::AssetType anAssetType, const char* aTypeName)
		{
			const ImGuiStyle& style = ImGui::GetStyle();
			const float clearWidth = ImGui::GetFrameHeight();
			const float boxWidth = (std::max)(50.f, ImGui::GetContentRegionAvail().x - clearWidth - style.ItemInnerSpacing.x);
			const ImVec2 boxSize(boxWidth, ImGui::GetFrameHeight());

			const ImVec2 boxMin = ImGui::GetCursorScreenPos();
			const ImVec2 boxMax(boxMin.x + boxSize.x, boxMin.y + boxSize.y);

			const ImGuiPayload* dragged = ImGui::GetDragDropPayload();
			const char* dndId = DragAndDrop::dnd_id[aFileType];
			const bool compatibleDrag = dragged && dragged->IsDataType(dndId);

			const bool clicked = ImGui::InvisibleButton("##asset_field", boxSize);
			const bool isHovered = ImGui::IsItemHovered();

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(boxMin, boxMax, ImGui::GetColorU32(isHovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), style.FrameRounding);

			if (compatibleDrag)
			{
				drawList->AddRect(boxMin, boxMax, ImGui::GetColorU32(ImGuiCol_DragDropTarget), style.FrameRounding, 0, 1.5f);
			}

			const std::string label = GetDisplayName(anAsset, aTypeName);

			const ImVec4 textColor = anAsset.IsValid() ? style.Colors[ImGuiCol_Text] : style.Colors[ImGuiCol_TextDisabled];
			const ImVec2 textPos(boxMin.x + style.FramePadding.x, boxMin.y + style.FramePadding.y);
			drawList->PushClipRect(boxMin, ImVec2(boxMax.x - style.FramePadding.x, boxMax.y), true);
			drawList->AddText(textPos, ImGui::GetColorU32(textColor), label.c_str());
			drawList->PopClipRect();

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dndId))
				{
					Assets::GUID guid;
					if (TryGetGUIDFromPayload(payload, guid))
						anAsset = Assets::AssetManager::Load<TAsset>(guid);
				}
				ImGui::EndDragDropTarget();
			}

			if (clicked)
				ImGui::OpenPopup("##AssetPicker");

			ImGui::SetNextWindowPos(ImVec2(boxMin.x, boxMax.y));
			ImGui::SetNextWindowSize(ImVec2(boxSize.x + style.ItemInnerSpacing.x + clearWidth, 0.f));
			Assets::GUID picked;
			if (DrawAssetPicker("##AssetPicker", anAssetType, anAsset.IsValid() ? anAsset.GetAssetID() : Assets::GUID(), picked))
				anAsset = picked.IsEmpty() ? TAsset() : Assets::AssetManager::Load<TAsset>(picked);

			ImGui::SameLine();
			if(ImGui::Button("X", ImVec2(clearWidth, clearWidth)))
				anAsset = TAsset();
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

		Register<Math::Color>([](void* v, const FieldEntry& entry) {
			
			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			ImGui::ColorPicker4("##value", (float*)addr);
			}
		);

		Register<Assets::AudioClip>([](void* v, const FieldEntry& entry) {
			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			DrawAssetField(*reinterpret_cast<Assets::AudioClip*>(addr), Utilities::FileInfo::FileType_Audio, Assets::AssetType::AudioClip, "Audio Clip");
			}
		);

		Register<Assets::Texture>([](void* v, const FieldEntry& entry) {
			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			DrawAssetField(*reinterpret_cast<Assets::Texture*>(addr), Utilities::FileInfo::FileType_Texture, Assets::AssetType::Texture, "Texture");
			}
		);

		Register<Assets::Material>([](void* v, const FieldEntry& entry) {
			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			DrawAssetField(*reinterpret_cast<Assets::Material*>(addr), Utilities::FileInfo::FileType_Material, Assets::AssetType::Material, "Material");
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