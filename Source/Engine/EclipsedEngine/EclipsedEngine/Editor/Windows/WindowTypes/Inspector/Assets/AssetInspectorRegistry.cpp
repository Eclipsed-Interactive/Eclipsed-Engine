#include "AssetInspectorRegistry.h"

#include "MaterialInspector.h"
#include "TextureInspector.h"
#include "AudioClipInspector.h"
#include "FontInspector.h"
#include "PixelShaderInspector.h"
#include "VertexShaderInspector.h"
#include "PrefabInspector.h"
#include "GenericFileInspector.h"

namespace Eclipse::Editor
{
	IInspector* AssetInspectorRegistry::Find(const std::filesystem::path& path)
	{
		for (auto& [type, inspector] : Get().inspectors)
		{
			if (inspector->CanInspect(path))
			{
				return inspector.get();
			}
		}

		return Get().fallbackInspector.get();
	}

	void AssetInspectorRegistry::SetFallback(std::unique_ptr<IInspector> inspector)
	{
		Get().fallbackInspector = std::move(inspector);
	}

	void AssetInspectorRegistry::RegisterAll()
	{
		Register<AudioClipInspector>();
		Register<FontInspector>();
		Register<MaterialInspector>();
		Register<PixelShaderInspector>();
		Register<PrefabInspector>();
		Register<TextureInspector>();
		Register<VertexShaderInspector>();
	}

	AssetInspectorRegistry& AssetInspectorRegistry::Get()
	{
		static AssetInspectorRegistry instance;
		return instance;
	}
}
