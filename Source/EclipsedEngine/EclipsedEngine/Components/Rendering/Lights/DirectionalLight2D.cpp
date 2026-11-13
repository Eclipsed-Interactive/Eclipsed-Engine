#include "DirectionalLight2D.h"

#include <Core/MainSingleton.h>

namespace Eclipse
{
	void DirectionalLight2D::OnComponentAdded()
	{
		MainSingleton::RegisterInstance<DirectionalLight2D>(*this);
	}
}
