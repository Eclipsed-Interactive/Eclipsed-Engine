#ifdef ECLIPSED_EDITOR
#include "EditorField.h"

#include "ImGui/imgui.h"
#include <random>

#include "CoreEngine/Math/Vector/Vector2.h"
#include "CoreEngine/Math/Vector/Vector3.h"
#include "CoreEngine/Math/Vector/Vector4.h"
#include "CoreEngine/Math/Color.h"

#include <vector>
#include <string>
#include "AssetEngine/Assets/Material.h"
#include "AssetEngine/Assets/AudioClip.h"
#include "AssetEngine/Assets/Prefab.h"

#include "PhysicsEngine/PhysicsEngineSettings.h"

#include "EclipsedEngine/Editor/Common/DragAndDrop.h"
#include "CoreEngine/Files/FileInfo.h"

#include "AssetEngine/Resources.h"
#include "AssetEngine/Editor/MetaFile/MetaFileRegistry.h"

namespace Eclipse::Editor
{
	template<>
	struct EditorFieldDrawer<int>
	{ 
		static bool Draw(int& aValue)
		{
			return ImGui::DragInt(GetFieldKey().c_str(), &aValue);
		}
	};

	template<>
	struct EditorFieldDrawer<float>
	{ 
		static bool Draw(float& aValue)
		{
			return ImGui::DragFloat(GetFieldKey().c_str(), &aValue);
		}
	};

	template<>
	struct EditorFieldDrawer<bool>
	{
		static bool Draw(bool& aValue)
		{
			return ImGui::Checkbox(GetFieldKey().c_str(), &aValue);
		}
	};

	template<typename U>
	struct EditorFieldDrawer<Math::Vector2<U>>
	{
		static bool Draw(Math::Vector2<U>& aValue)
		{
			return ImGui::DragScalarN(GetFieldKey().c_str(), GetImGuiType<U>(), &aValue.x, 2);
		}
	};

	template<typename U>
	struct EditorFieldDrawer<Math::Vector3<U>>
	{
		static bool Draw(Math::Vector3<U>& aValue)
		{
			return ImGui::DragScalarN(GetFieldKey().c_str(), GetImGuiType<U>(), &aValue.x, 3);
		}
	};

	template<typename U>
	struct EditorFieldDrawer<Math::Vector4<U>>
	{
		static bool Draw(Math::Vector4<U>& aValue)
		{
			return ImGui::DragScalarN(GetFieldKey().c_str(), GetImGuiType<U>(), &aValue.x, 4);
		}
	};

	template<typename U, typename Alloc>
	struct EditorFieldDrawer<std::vector<U, Alloc>>
	{
		static bool Draw(std::vector<U, Alloc>& aValue)
		{
			return false;
		}
	};

	template<>
	struct EditorFieldDrawer<Math::Color>
	{
		static bool Draw(Math::Color& aValue)
		{
			return ImGui::ColorPicker4(GetFieldKey().c_str(), &aValue.r);
		}
	};

	template<>
	struct EditorFieldDrawer<std::string>
	{
		static bool Draw(std::string& aValue)
		{
			char TemporaryName[256];
			std::strcpy(TemporaryName, aValue.c_str());

			if (ImGui::InputText(GetFieldKey().c_str(), TemporaryName, 256, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				memcpy(aValue.data(), &TemporaryName, strlen(TemporaryName) + 1);
				return true;
			}

			return false;
		}
	};

	template<>
	struct EditorFieldDrawer<Material>
	{
		static bool Draw(Material& aValue)
		{
			std::string name = "No material.";

			if (aValue.IsValid())
			{
				name = aValue.GetAssetID();
			}

			if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Material))
			{
				std::string guid = MetaFileRegistry::GetGUID(Editor::DragAndDrop::payloadBuffer);
				aValue = Resources::Get<Material>(guid);

				aValue.Create();
				return true;
			}

			return false;
		}
	};

	template<>
	struct EditorFieldDrawer<Prefab>
	{
		static bool Draw(Prefab& aValue)
		{
			return false;
		}
	};

	template<>
	struct EditorFieldDrawer<AudioClip>
	{
		static bool Draw(AudioClip& aValue)
		{
			return false;
		}
	};

	template<>
	struct EditorFieldDrawer<Texture>
	{
		static bool Draw(Texture& aValue)
		{
			std::string name = "No texture.";

			if (aValue.IsValid())
			{
				name = aValue.GetAssetID();
			}

			if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Texture))
			{
				std::string guid = MetaFileRegistry::GetGUID(Editor::DragAndDrop::payloadBuffer);
				aValue = Resources::Get<Texture>(guid);
				return true;
			}

			return false;
		}
	};

	template<>
	struct EditorFieldDrawer<Layer>
	{
		static bool Draw(Layer& aValue)
		{
			return false;
		}
	};
}
#endif