#include "Forcelink.h"

#include "Source/Player.h"
#include "Source/Simon.h"



CREATE_COMPONENT(Player);
CREATE_COMPONENT(Simon);

static ComponentEntry g_components[] = {
	COMPONENT_ENTRY(Player)
	COMPONENT_ENTRY(Simon)
};

ComponentRegistrySnapshot __cdecl RegisterComponents()
{
	ComponentRegistrySnapshot snapshot;

	snapshot.components = g_components;
	snapshot.count = static_cast<int>(sizeof(g_components) / sizeof(g_components[0]));

	return snapshot;
}
