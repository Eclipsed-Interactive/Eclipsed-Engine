#include "ArtifactReader.h"

#include <fstream>

namespace Eclipse::AssetSystem
{
	ArtifactReader::ArtifactReader(std::span<const Byte> aBytes)
		: myBytes(aBytes) { }

	bool ArtifactReader::Read(void* outData, Size aSize)
	{
		if (aSize == 0) return true;
		if (aSize + myOffset > Capacity()) return false;

		memcpy(outData, myBytes.data() + myOffset, aSize);
		myOffset += aSize;
		return true;
	}

	void ArtifactReader::Skip(Size aSize)
	{
		myOffset += aSize;
	}


	Size ArtifactReader::Capacity()
	{
		return myBytes.size();
	}

	const void* ArtifactReader::Data() const
	{
		return myBytes.data();
	}

	const void* ArtifactReader::Current() const
	{
		return myBytes.data() + myOffset;
	}

	const Size& ArtifactReader::Offset() const
	{
		return myOffset;
	}
}