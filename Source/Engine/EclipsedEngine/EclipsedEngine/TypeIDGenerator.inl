#include "TypeIDGenerator.h"

namespace Eclipse
{
	template<typename T>
	TypeID TypeRegistry::GetTypeID()
	{
		static TypeID id = gTypeRegistryNextID++;
		return id;
	}
}