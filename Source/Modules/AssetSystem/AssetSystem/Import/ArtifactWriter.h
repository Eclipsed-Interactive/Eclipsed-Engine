#pragma once

#include <Core/Standard/Core/Types.hpp>
#include <vector>
#include <span>

namespace Eclipse::AssetSystem
{
	class ArtifactWriter
	{
	public:
		void Write(const void* data, Size size);
		void SaveToFile(const std::filesystem::path& aPath);

		std::span<const Byte> GetBytes() const;

	private:
		std::vector<Byte> bytes;
	};
}