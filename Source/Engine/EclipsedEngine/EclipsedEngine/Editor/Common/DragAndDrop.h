#pragma once

#ifdef ECLIPSED_EDITOR

#include <vector>
#include <unordered_map>

#include "CoreEngine/Math/Math.h"
#include "CoreEngine/Files/FileInfo.h"

namespace Eclipse::Editor
{
	class DragAndDrop
	{
	public:
		static bool BeginTarget(const char* text, const Utilities::FileInfo::FileType_& type, bool* textClicked = nullptr, bool* arrowClicked = nullptr);

		static void BeginSource(const char* payload, const size_t& size, const Utilities::FileInfo& info);

	private:
		static bool TextBox(const char* text, Math::Vector2f padding, const Math::Color& aBgColor, const Math::Color& aBorderColor, const char* id, bool* boxClicked = nullptr, bool* dropDownClicked = nullptr);


	public:
		static inline char payloadBuffer[512];
		static inline int index = 0;

		static inline std::unordered_map<Utilities::FileInfo::FileType_, const char*> dnd_id =
		{
			{Utilities::FileInfo::FileType_Audio, "DND_AUDIO"},
			{Utilities::FileInfo::FileType_Texture, "DND_TEXTURE"},
			{Utilities::FileInfo::FileType_Scene, "DND_SCENE"},
			{Utilities::FileInfo::FileType_VertexShader, "DND_VERTEX_SHADER"},
			{Utilities::FileInfo::FileType_PixelShader, "DND_PIXEL_SHADER"},
			{Utilities::FileInfo::FileType_Material, "DND_MATERIAL"},
			{Utilities::FileInfo::FileType_Prefab, "DND_PREFAB"},
			{Utilities::FileInfo::FileType_Font, "DND_FONT"},
		};
	};
}

#endif