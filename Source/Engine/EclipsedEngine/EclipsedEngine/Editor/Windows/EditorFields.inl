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
#include "AssetEngine/AssetManager.h"

#include "PhysicsEngine/PhysicsEngineSettings.h"

#include "EclipsedEngine/Editor/Common/DragAndDrop.h"
#include "CoreEngine/Files/FileInfo.h"

#include "CoreEngine/MainSingleton.h"

namespace Eclipse::Editor
{
	template<>
	struct EditorFieldDrawer<int>
	{ 
		static bool DrawField(int& aValue)
		{
			return ImGui::DragInt(GetFieldKey().c_str(), &aValue);
		}
	};

	template<>
	struct EditorFieldDrawer<float>
	{ 
		static bool DrawField(float& aValue)
		{
			return ImGui::DragFloat(GetFieldKey().c_str(), &aValue);
		}
	};

	template<>
	struct EditorFieldDrawer<bool>
	{
		static bool DrawField(bool& aValue)
		{
			return ImGui::Checkbox(GetFieldKey().c_str(), &aValue);
		}
	};

	template<typename U>
	struct EditorFieldDrawer<Math::Vector2<U>>
	{
		static bool DrawField(Math::Vector2<U>& aValue)
		{
			return ImGui::DragScalarN(GetFieldKey().c_str(), GetImGuiType<U>(), &aValue.x, 2);
		}
	};

	template<typename U>
	struct EditorFieldDrawer<Math::Vector3<U>>
	{
		static bool DrawField(Math::Vector3<U>& aValue)
		{
			return ImGui::DragScalarN(GetFieldKey().c_str(), GetImGuiType<U>(), &aValue.x, 3);
		}
	};

	template<typename U>
	struct EditorFieldDrawer<Math::Vector4<U>>
	{
		static bool DrawField(Math::Vector4<U>& aValue)
		{
			return ImGui::DragScalarN(GetFieldKey().c_str(), GetImGuiType<U>(), &aValue.x, 4);
		}
	};

	template<typename U, typename Alloc>
	struct EditorFieldDrawer<std::vector<U, Alloc>>
	{
		static bool DrawField(std::vector<U, Alloc>& aValue)
		{
			return false;
		}
	};

	template<>
	struct EditorFieldDrawer<Math::Color>
	{
		static bool DrawField(Math::Color& aValue)
		{
			return ImGui::ColorPicker4(GetFieldKey().c_str(), &aValue.r);
		}
	};

	template<>
	struct EditorFieldDrawer<std::string>
	{
		static bool DrawField(std::string& aValue)
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

	/*template<>
	struct EditorFieldDrawer<Assets::Material, Assets::MaterialData>
	{
		static bool DrawField(Assets::Material& aValue)
		{
			std::string name = "No material.";

			if (aValue.IsValid())
			{
				name = aValue.GetAssetID();
			}

			if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Material))
			{
				size_t guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetMetaData(Editor::DragAndDrop::payloadBuffer).guid;
				aValue = Resources::Get<Material>(guid);

				aValue.Create();
				return true;
			}

			return false;
		}

		static bool DrawEditor(SerializedMaterial& aValue)
		{
			bool wasChanged = false;
			std::string psName = std::to_string(aValue.pixelShaderGuid);
			if (Editor::DragAndDrop::BeginTarget(std::string("Pixel Shader: " + psName).c_str(), Utilities::FileInfo::FileType_PixelShader))
			{
				size_t guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetMetaData(Editor::DragAndDrop::payloadBuffer).guid;
				aValue.pixelShaderGuid = guid;
				wasChanged = true;
			}

			ImGui::Spacing();

			std::string vsName = std::to_string(aValue.vertexShaderGuid);
			if (Editor::DragAndDrop::BeginTarget(std::string("Vertex Shader: " + vsName).c_str(), Utilities::FileInfo::FileType_VertexShader))
			{
				size_t guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetMetaData(Editor::DragAndDrop::payloadBuffer).guid;
				aValue.vertexShaderGuid = guid;
				wasChanged = true;
			}

			ImGui::Spacing();

			std::string tName = std::to_string(aValue.textureGuid);
			if (Editor::DragAndDrop::BeginTarget(std::string("Texture: " + tName). c_str(), Utilities::FileInfo::FileType_Texture))
			{
				size_t guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetMetaData(Editor::DragAndDrop::payloadBuffer).guid;
				aValue.textureGuid = guid;
				wasChanged = true;
			}

			return wasChanged;
		}
	};

	template<>
	struct EditorFieldDrawer<Prefab>
	{
		static bool DrawField(Prefab& aValue)
		{
			return false;
		}

		static bool DrawEditor(Prefab& aValue)
		{
			ImGui::Text("Wallahi");

			return false;
		}
	};

	template<>
	struct EditorFieldDrawer<AudioClip>
	{
		static bool DrawField(AudioClip& aValue)
		{
			return false;
		}

		static bool DrawEditor(AudioClip& aValue)
		{
			ImGui::Text("Wallahi");

			return false;
		}
	};

	template<>
	struct EditorFieldDrawer<Texture>
	{
		static bool DrawField(Texture& aValue)
		{
			std::string name = "No texture.";

			if (aValue.IsValid())
			{
				name = aValue.GetAssetID();
			}

			if (Editor::DragAndDrop::BeginTarget(name.c_str(), Utilities::FileInfo::FileType_Texture))
			{
				size_t guid = MainSingleton::GetInstance<Assets::AssetDatabase>().GetMetaData(Editor::DragAndDrop::payloadBuffer).guid;
				aValue = Resources::Get<Texture>(guid);
				return true;
			}

			return false;
		}

		static bool DrawEditor(Texture& aValue)
		{
			ImGui::Text(std::to_string(aValue.GetAssetID()).c_str());

			return false;
		}
	};*/

	template<>
	struct EditorFieldDrawer<Layer>
	{
		static bool DrawField(Layer& aValue)
		{
			return false;
		}
	};
}
#endif