#include "EditorReflectionDrawHelper.h"

#include "ImGui/imgui.h"

template<typename T>
inline void EditorReflectionDrawHelper::DrawReflectedVariable(Eclipse::Reflection::SerializedVariable<T>& var)
{
	if constexpr (Is_Vector<T>::value || Is_Array<T>::value)
	{
		var.isDrawn = ImGui::CollapsingHeader(var.GetName());

		if (!var.isDrawn) return;

		var.ResolveTypeInfo();
		for (int i = 0; i < var.GetCount(); ++i)
		{
			ImGui::Dummy({ 20, 0 });
			ImGui::SameLine();

			ImGui::PushID(i);

			if constexpr (Is_Vector<T>::value)
			{
				ImGui::SameLine();

				if (ImGui::Button("-"))
				{
					var.data.erase(var.data.begin() + i);
					ImGui::PopID();
					return;
				}

				ImGui::SameLine();
			}

			var.DrawElement(var.data[i]);
			ImGui::PopID();
		}

		if constexpr (Is_Vector<T>::value)
		{
			ImGui::Dummy({ 20, 0 });
			ImGui::SameLine();

			if (ImGui::Button("Add"))
			{
				var.data.push_back({});
			}
		}
	}
	else
	{
		auto name = var.GetName();

		ImGui::Text(name);
		ImGui::SameLine();
		var.DrawElement(var.data);
	}
}