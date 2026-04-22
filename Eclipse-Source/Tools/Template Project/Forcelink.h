#pragma once

#define CREATE_COMPONENT(TYPE)														\
Eclipse::Component* Create_##TYPE(unsigned char* address) {	\
	return new(address)TYPE();		\
}


#define COMPONENT_ENTRY(TYPE) { #TYPE, &Create_##TYPE, sizeof(TYPE) },

namespace Eclipse
{
	class Component;
}

struct ComponentEntry final
{
	const char* name;
	Eclipse::Component* (__cdecl* createFunc)(unsigned char* address);
	size_t size;
};

struct ComponentRegistrySnapshot final
{
	ComponentEntry* components = nullptr;
	int count = 0;
};

namespace Eclipse::Utilities
{
	class MainSingleton;
}

extern "C"
{
	__declspec(dllexport) ComponentRegistrySnapshot __cdecl RegisterComponents();
}