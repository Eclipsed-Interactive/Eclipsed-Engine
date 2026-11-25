#pragma once

#include <unordered_map>
#include <vector>
#include <Core/GUID.h>

namespace Eclipse::Editor
{
	struct Preview
	{
		GUID guid;
		unsigned textureId;
		int width;
		int height;
		unsigned long long lastFrameUsed = 0;
	};

	class PreviewManager final
	{
	public:
		static void Init();

		static void Update();

	public:
		static unsigned int GetPreview(GUID aGuid);
		static const Preview& GetPreviewData(GUID aGuid);
		static const Preview* TryGetPreviewData(GUID aGuid);

		static const Preview& LoadPreview(GUID aGuid);

	private:
		static inline constexpr unsigned long long FramesToLive = 100;
		static inline unsigned long long CurrentFrame = 0;
		static inline std::vector<Preview> ActivePreviews;
	};
}