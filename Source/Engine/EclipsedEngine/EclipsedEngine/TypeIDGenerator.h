#pragma once

#include <unordered_map>

#include "CoreEngine/Macros/defines.h"

namespace Eclipse
{
	using TypeID = unsigned;

	class ECLIPSED_API TypeRegistry
	{
	public:
		template<typename T>
		static TypeID GetTypeID();
	};

	inline unsigned gTypeRegistryNextID = 1;
}



#include "TypeIDGenerator.inl"