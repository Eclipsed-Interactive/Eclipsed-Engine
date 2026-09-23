#include "EditorLayout.h"

#include "Views/Types/AssetBrowserView.h"
#include "Views/Types/Inspector/InspectorView.h"
#include "Views/Types/GameView.h"
#include "Views/Types/SceneView.h"
#include "Views/Types/HierarchyView.h"
#include "Views/Types/SettingsView.h"

#include <regex>
#include "ImGui/imgui.h"
#include "Core/Settings/EditorSettings.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace Eclipse::Editor
{
	void EditorLayout::RegisterViewTypes()
	{
		registry.RegisterView<AssetBrowserView>();
		registry.RegisterView<InspectorView>();
		registry.RegisterView<GameView>();
		registry.RegisterView<SceneView>();
		registry.RegisterView<HierarchyView>();
		registry.RegisterView<SettingsView>();
	}


	void EditorLayout::Init()
	{
		EditorLayout::RegisterViewTypes();

		if (std::filesystem::exists("CurrentLayout.layout"))
		{
			LoadLayout("CurrentLayout.layout");
		}
		else
		{
			LoadLayoutFromMemory(DefaultLayout);
		}
	}

	void EditorLayout::Shutdown()
	{
		SaveLayout("CurrentLayout.layout");
	}

	void EditorLayout::EndFrame()
	{

	}

	void EditorLayout::Draw()
	{
		for (auto it = activeViews.begin(); it < activeViews.end();)
		{
			EditorViewInstance* instance = *it;

			if (!instance->open)
			{
				ImGui::ClearWindowSettings(instance->idString.c_str());

				delete instance;
				it = activeViews.erase(it);
				continue;
			}

			++it;
		}

		if (!layoutToOpen.empty())
			LoadLayout(layoutToOpen.c_str());

		ImGui::DockSpaceOverViewport(
			1,
			ImGui::GetMainViewport(),
			ImGuiDockNodeFlags_PassthruCentralNode
		);

		for (auto instance : activeViews)
		{
			std::string windowName = std::string(instance->view->GetName()) + "##" + std::to_string(instance->id);

			if (ImGui::Begin(windowName.c_str(), &instance->open))
			{
				instance->view->Draw();
			}
			ImGui::End();
		}
	}

	void EditorLayout::OpenLayout(std::string_view layoutName)
	{
		layoutName = layoutName;
	}

	void EditorLayout::OpenView(std::string_view viewName, int id)
	{
		const EditorViewType* type = registry.GetViewType(viewName);
		if (!type) return;

		EditorViewInstance* instance = type->create(id);
		activeViews.push_back(instance);

		instance->view->SetID(std::string(instance->view->GetName()) + "##" + std::to_string(instance->id));
		instance->view->OnOpen();
	}

	EditorViewRegistry& EditorLayout::GetViewRegistry()
	{
		return registry;
	}

	bool EditorLayout::LoadLayout(const char* layout)
	{
		if (LoadLayoutFromMemory(layout)) return true;
		else if (LoadLayoutFromDisk(layout)) return true;
		else if (LoadLayoutFromProject(layout)) return true;
		else return false;
	}

	bool EditorLayout::LoadLayoutFromMemory(const char* layout)
	{
		rapidjson::Document doc;
		if (doc.Parse(layout).HasParseError())
			return false;

		if (!doc.HasMember("ini") || !doc["ini"].IsString())
			return false;

		const std::string ini = doc["ini"].GetString();

		if (!doc.HasMember("views") && !doc["views"].IsArray())
			return false;

		const rapidjson::Value& views = doc["views"];

		for (rapidjson::SizeType i = 0; i < views.Size(); ++i)
		{
			const rapidjson::Value& view = views[i];

			if (!view.IsObject())
				return false;

			if (!view.HasMember("name") || !view["name"].IsString())
				return false;

			if (!view.HasMember("id") || !view["id"].IsInt())
				return false;

			const int id = view["id"].GetInt();
			const std::string name = view["name"].GetString();

			OpenView(name, id);
		}

		ImGui::LoadIniSettingsFromMemory(ini.c_str(), ini.size());
		return true;
	}

	bool EditorLayout::LoadLayoutFromDisk(const char* path)
	{
		if (!path || !std::filesystem::exists(path))
			return false;

		if (strcmp(path, "CurrentLayout.layout"))
		{
			std::filesystem::copy_file(
				path,
				"CurrentLayout.layout",
				std::filesystem::copy_options::overwrite_existing
			);
		}

		std::ifstream file("CurrentLayout.layout", std::ios::binary);
		std::string content = {
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>()
		};

		return LoadLayoutFromMemory(content.c_str());
	}

	bool EditorLayout::LoadLayoutFromProject(const char* layoutName)
	{
		const std::filesystem::path path = PathManager::GetProjectRoot() / "Editor" / "Layouts" / (std::string(layoutName) + ".layout");
		if (std::filesystem::exists(path))
			return false;

		return LoadLayoutFromDisk(path.generic_string().c_str());
	}








	void EditorLayout::SaveLayout(const char* layoutPath)
	{
		rapidjson::Document doc;
		doc.SetObject();

		auto& alloc = doc.GetAllocator();

		const char* ini = ImGui::SaveIniSettingsToMemory();

		doc.AddMember(
			"ini",
			rapidjson::Value(ini, alloc),
			alloc
		);

		rapidjson::Value openViews(rapidjson::kArrayType);

		rapidjson::Value views(rapidjson::kArrayType);

		for (auto* view : activeViews)
		{
			rapidjson::Value obj(rapidjson::kObjectType);

			obj.AddMember("name",
				rapidjson::Value(view->view->GetName(), alloc),
				alloc);

			obj.AddMember("id", view->id, alloc);

			views.PushBack(obj, alloc);
		}

		doc.AddMember("views", views, alloc);

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		std::ofstream file(layoutPath);
		file << buffer.GetString();
	}

	void EditorLayout::SaveNewLayout(std::string_view layoutName)
	{
		const std::string path = (PathManager::GetProjectRoot() / "Editor/Layouts" / layoutName).generic_string() + ".layout";
		SaveLayout(path.c_str());
	}
}