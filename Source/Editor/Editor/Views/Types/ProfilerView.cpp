#include "ProfilerView.h"

#include "ImGui/imgui.h"
#include <Core/Profiling/ProfilerManager.h>

#include <Core/Profiling/ProfilerNode.h>
#include <Core/Math/Random.h>

namespace Eclipse::Editor
{
	namespace {
		void FormatTime(std::string& time, std::chrono::microseconds t)
		{
			const double microseconds = (double)t.count();
			if (microseconds >= 60'000'000)		time = "fuck....";
			else if (microseconds >= 1'000'000)	time = std::format("{:.2f}s", microseconds / 1'000'000);
			else if (microseconds >= 1'000)		time = std::format("{:.2f}ms", microseconds / 1'000);
			else								time = std::format("{:.2f}µs", microseconds);
		}
	}

	void ProfilerView::OnOpen()
	{

	}

	void ProfilerView::Draw()
	{
		const char* phaseNames [] = { "Init", "Update" };
		for (size_t i = 0; i < (size_t)Profiler::Phase::Count; i++)
		{
			const Profiler::CallTree& tree = Profiler::ProfilerManager::Completed((Profiler::Phase)i);
			if (!tree.GetRoot()) 
				continue;
			
			std::string time;
			FormatTime(time, tree.GetRoot()->duration);

			if (ImGui::TreeNodeEx(std::to_string(i).c_str(), 0, "%s - %s", phaseNames[i], time.c_str()))
			{
				DrawNode(tree.GetRoot());
				
				ImGui::TreePop();
			}

			if (i < (size_t)Profiler::Phase::Count)
			{
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Spacing();
			}
		}

	}

	void ProfilerView::DrawNode(const Profiler::ProfilerNode* node)
	{
		if (!node) return;  

		ImGuiTreeNodeFlags flags = node->children.empty() ? ImGuiTreeNodeFlags_Leaf : 0;

		std::string time;
		FormatTime(time, node->duration);


		if (16666.0 <= node->duration.count())
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));

		bool open = ImGui::TreeNodeEx(node->functionName.c_str(), flags, "%s  %s", node->functionName.c_str(), time.c_str());

		if (16666.0 <= node->duration.count())
			ImGui::PopStyleColor();

		if (open)
		{
			for (const Profiler::ProfilerNode* child : node->children)
				DrawNode(child);

			ImGui::TreePop();
		}
	}
}