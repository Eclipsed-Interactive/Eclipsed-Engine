#include "PrefabLoader.h"



namespace Eclipse
{
	PrefabData* PrefabLoader::Load(std::ifstream& in) const
	{
		PrefabData* data = new PrefabData;
		int prefabSize = 0;

		in.read(reinterpret_cast<char*>(&prefabSize), sizeof(int));

		data->data = reinterpret_cast<char*>(malloc(prefabSize + 1));
		in.read(data->data, prefabSize);
		memset(data->data + prefabSize, '\0', 1);

		return data;
	}
}