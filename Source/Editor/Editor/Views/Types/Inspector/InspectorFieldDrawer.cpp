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

#include "Reflection/Reflection.hpp"
#include "Reflection/TypeRegistry.h"

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


		void InvokeOnChange(void* anObject, const Reflection::FieldEntry& anEntry)
		{
			if (const Reflection::OnChange* onChange = anEntry.attributeContainer.TryGet<Reflection::OnChange>())
			{
				if (onChange->event)
					onChange->event(anObject);
			}
		}

		template<typename TAsset>
		bool AssetEquals(const TAsset& a, const TAsset& b)
		{
			if (a.IsValid() != b.IsValid()) return false;
			if (!a.IsValid()) return true;
			return a.GetAssetID() == b.GetAssetID();
		}

		// Returns true if the asset was changed this frame.
		template<typename TAsset>
		bool DrawAssetField(TAsset& anAsset, Utilities::FileInfo::FileType_ aFileType, Assets::AssetType anAssetType, const char* aTypeName)
		{
			TAsset newAsset = anAsset;
			bool assigned = false;

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
					{
						newAsset = Assets::AssetManager::Load<TAsset>(guid);
						assigned = true;
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (clicked)
				ImGui::OpenPopup("##AssetPicker");

			ImGui::SetNextWindowPos(ImVec2(boxMin.x, boxMax.y));
			ImGui::SetNextWindowSize(ImVec2(boxSize.x + style.ItemInnerSpacing.x + clearWidth, 0.f));
			Assets::GUID picked;
			if (DrawAssetPicker("##AssetPicker", anAssetType, anAsset.IsValid() ? anAsset.GetAssetID() : Assets::GUID(), picked))
			{
				newAsset = picked.IsEmpty() ? TAsset() : Assets::AssetManager::Load<TAsset>(picked);
				assigned = true;
			}

			ImGui::SameLine();
			if (ImGui::Button("X", ImVec2(clearWidth, clearWidth)))
			{
				newAsset = TAsset();
				assigned = true;
			}

			if (!assigned || AssetEquals(anAsset, newAsset))
				return false;

			anAsset = newAsset;
			return true;
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
			int data = *reinterpret_cast<int*>(addr);

			bool changed = range
				? ImGui::SliderInt("##value", &data, range->min, range->max)
				: ImGui::DragInt("##value", &data, step);

			if (changed) Set(entry.owner, entry.name, v, data);

			}
		);

		Register<float>([](void* v, const FieldEntry& entry) { // works as it should. Copy this to all other.
			const Range* range = entry.attributeContainer.TryGet<Range>();

			float step = 0.01f;
			const Step* stepAttr = entry.attributeContainer.TryGet<Step>();
			if (stepAttr) step = stepAttr->step;

			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			float data = *reinterpret_cast<float*>(addr);

			bool changed = range
				? ImGui::SliderFloat("##value", &data, range->min, range->max)
				: ImGui::DragFloat("##value", &data, step);

			if (changed) Set(entry.owner, entry.name, v, data);

			}
		);

		Register<bool>([](void* v, const FieldEntry& entry) {
			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			bool data = *reinterpret_cast<bool*>(addr);

			bool changed = ImGui::Checkbox("##value", (bool*)addr);
			if (changed) Set(entry.owner, entry.name, v, data);

			}
		);

		Register<Math::Vector2f>([](void* v, const FieldEntry& entry) {
			const Range* range = entry.attributeContainer.TryGet<Range>();

			float step = 0.1f;
			const Step* stepAttr = entry.attributeContainer.TryGet<Step>();
			if (stepAttr) step = stepAttr->step;

			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			Math::Vector2f data = *reinterpret_cast<Math::Vector2f*>(addr);

			bool changed = range
				? ImGui::SliderFloat2("##value", &data.x, range->min, range->max)
				: ImGui::DragFloat2("##value", &data.x, step);

			if (changed) Set(entry.owner, entry.name, v, data);

			}
		);

		Register<Math::Vector2d>([](void* v, const FieldEntry& entry) {
			const Range* range = entry.attributeContainer.TryGet<Range>();

			float step = 0.1f;
			const Step* stepAttr = entry.attributeContainer.TryGet<Step>();
			if (stepAttr) step = stepAttr->step;

			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			Math::Vector2d data = *reinterpret_cast<Math::Vector2d*>(addr);

			bool changed = range
				? ImGui::SliderScalarN("##value", ImGuiDataType_Double, &data.x, 2, &range->min, &range->max)
				: ImGui::DragScalarN("##value", ImGuiDataType_Double, &data.x, 2, step);

			if (changed) Set(entry.owner, entry.name, v, data);

			}
		);

		Register<Math::Color>([](void* v, const FieldEntry& entry) {

			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			Math::Color data = *reinterpret_cast<Math::Color*>(addr);

			bool changed = ImGui::ColorPicker4("##value", &data.r);
			if (changed) Set(entry.owner, entry.name, v, data);
			}
		);

		Register<Assets::AudioClip>([](void* v, const FieldEntry& entry) {
			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			if (DrawAssetField(*reinterpret_cast<Assets::AudioClip*>(addr), Utilities::FileInfo::FileType_Audio, Assets::AssetType::AudioClip, "Audio Clip"))
				InvokeOnChange(v, entry);
			}
		);

		Register<Assets::Texture>([](void* v, const FieldEntry& entry) {
			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			if (DrawAssetField(*reinterpret_cast<Assets::Texture*>(addr), Utilities::FileInfo::FileType_Texture, Assets::AssetType::Texture, "Texture"))
				InvokeOnChange(v, entry);
			}
		);

		Register<Assets::Material>([](void* v, const FieldEntry& entry) {
			unsigned char* addr = reinterpret_cast<unsigned char*>(v) + entry.offset;
			if (DrawAssetField(*reinterpret_cast<Assets::Material*>(addr), Utilities::FileInfo::FileType_Material, Assets::AssetType::Material, "Material"))
				InvokeOnChange(v, entry);
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