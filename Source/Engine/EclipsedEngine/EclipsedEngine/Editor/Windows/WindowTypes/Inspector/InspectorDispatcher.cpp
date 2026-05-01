#include "InspectorDispatcher.h"

#include "CoreEngine/Utility/VariantUtils.h"
#include "Assets/AssetInspectorRegistry.h"
#include "World/GameObjectInspector.h"

namespace Eclipse::Editor
{
	void InspectorDispatcher::Draw(const InspectableTarget& target)
	{
		std::visit(Utility::overloaded
			{
				[](std::monostate) {},
				[](AssetTarget a) {if(IInspector* i = AssetInspectorRegistry::Find(a)) i->Draw(a);},
				[](GameObjectTarget g) {GameObjectInspector::Draw(g); }
			}, target);

	}
}