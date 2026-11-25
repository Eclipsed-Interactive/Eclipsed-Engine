#include "OpenGL_InstancedRenderer.h"

#include "EclipsedEngine/Components/Rendering/SpriteRenderer2D.h"

#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"
#include "Renderer/RendererManager.h"

#include <OpenGL/glad/glad.h>

namespace Eclipse::Graphics::OpenGL
{
	void OpenGL_InstancedRenderer::Init()
	{
		//myInstanceMaterial = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::MATERIAL_2D_INSTANCE_SPRITE);
	}

	void OpenGL_InstancedRenderer::AddRenderObject(TransformBuffer* aTransformBuffer, SpriteBuffer* aSpriteBuffer, Assets::Material* aMaterial, Assets::Texture* aSprite)
	{
		InstancedRenderObject::InternalInstanceTransform transform;
		transform.Position = aTransformBuffer->Position;
		transform.Scale = aTransformBuffer->Scale;
		transform.Rotation = aTransformBuffer->Rotation;

		Assets::GUID AssetID = {0};

		if (aSprite->IsValid())
			AssetID = aSprite->GetAssetID();
		
		InstancedRenderObject& object = RenderComponents[AssetID].emplace_back();
		object.TransformBuffer = aTransformBuffer;
		object.SpriteBuffer = aSpriteBuffer;
		object.Material = aMaterial;
		object.Sprite = aSprite;
		object.InstanceTransform = transform;
	}

	void OpenGL_InstancedRenderer::ExecuteStaticRendering()
	{
		static Assets::Material myInstanceMaterial = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::MATERIAL_2D_INSTANCE_SPRITE);

		auto* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();
		auto* graphicsDevice = Graphics::RendererManager::GetRenderer().GetDevice();

		for (auto& [AssetID, RenderVector] : RenderComponents)
		{
			if (RenderVector.empty())
				continue;

			InstancedRenderObject& FirstRenderObject = RenderVector.back();

			graphicsDevice->BindMaterial(*FirstRenderObject.Material);
			graphicsDevice->BindShader(myInstanceMaterial.GetProgramID());
			if (FirstRenderObject.Sprite->IsValid())
				graphicsDevice->BindTexture(0, *FirstRenderObject.Sprite);

			buffer->SetOrCreateBuffer(Graphics::GraphicsBufferEnum::g_SpriteBuffer, 3, FirstRenderObject.SpriteBuffer);
			buffer->SetOrCreateBuffer(Graphics::GraphicsBufferEnum::g_MaterialBuffer, 5, &FirstRenderObject.Material->GetBuffer());

			std::vector<InstancedRenderObject::InternalInstanceTransform> instanceTransform;
			for (auto& RenderComponent : RenderVector)
				instanceTransform.emplace_back(RenderComponent.InstanceTransform);

			Graphics::IRenderer& renderer = Graphics::RendererManager::GetRenderer();

			glBindBuffer(GL_ARRAY_BUFFER, renderer.GetDrawer()->mySprite->GetInstanceVBOID());
			glBufferData(GL_ARRAY_BUFFER, instanceTransform.size() * sizeof(Sprite::InstanceTransform), instanceTransform.data(), GL_DYNAMIC_DRAW);

			unsigned VBOID = renderer.GetDrawer()->mySprite->GetSpriteBufferID();
			glBindVertexArray(VBOID);
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instanceTransform.size());
			glBindVertexArray(0);
		}
	}
}