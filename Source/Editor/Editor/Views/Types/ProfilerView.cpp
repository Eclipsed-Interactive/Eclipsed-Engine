#include "ProfilerView.h"

#include "ImGui/imgui.h"
#include <Core/Profiling/ProfilerManager.h>

#include <Core/Profiling/ProfilerNode.h>

namespace Eclipse::Editor
{
	namespace {
		void FormatTime(std::string& time, std::chrono::microseconds t)
		{
			const double us = (double)t.count();
			if (us >= 60'000'000)		time = std::format("{:.2f}m", us / 60'000'000);
			else if (us >= 1'000'000)	time = std::format("{:.2f}s", us / 1'000'000);
			else if (us >= 1'000)		time = std::format("{:.2f}ms", us / 1'000);
			else						time = std::format("{:.2f}us", us);
		}
	}

	void ProfilerView::OnOpen()
	{

	}

	void ProfilerView::Draw()
	{
		for (size_t i = 0; i < (size_t)Profiler::Phase::Count; i++)
		{
			const Profiler::CallTree& tree = Profiler::ProfilerManager::Completed((Profiler::Phase)i);
			
			if (ImGui::TreeNodeEx(std::to_string(i).c_str()))
			{
				S(tree.GetRoot());
				
				ImGui::TreePop();
			}

		}

	}

	void ProfilerView::S(const Profiler::ProfilerNode* node)
	{
		if (!node) return;                       // empty tree, nothing recorded yet

		ImGuiTreeNodeFlags flags = node->children.empty() ? ImGuiTreeNodeFlags_Leaf : 0;

		std::string time;
		FormatTime(time, node->duration);

		if (ImGui::TreeNodeEx(node->functionName.c_str(), flags, "%s  %s",
			node->functionName.c_str(), time.c_str()))
		{
			for (const Profiler::ProfilerNode* child : node->children)
				S(child);

			ImGui::TreePop();
		}
	}
}