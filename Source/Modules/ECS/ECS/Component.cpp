#include "ECS/Component.h"

namespace Eclipse
{
	TypeID Component::GetID()
	{
		return Type::GetTypeID<Component>();
	}

}