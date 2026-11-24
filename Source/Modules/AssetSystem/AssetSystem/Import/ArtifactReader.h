#pragma once

#include <Core/Standard/Core/Types.hpp>
#include <vector>
#include <span>

#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
	class ASSETSYSTEM_API ArtifactReader
	{
	public:
		ArtifactReader(std::span<const Byte> aBytes);

	public:
		bool Read(void* outData, Size aSize);
		Size Capacity();

		const void* Data() const;
		const void* Current() const;


		const Size& Offset() const;
		void Skip(Size aSize);

	private:
		std::span<const Byte> myBytes;
		Size myOffset = 0;
	};
}