#include "OpenGL_InstancedRenderer.h"

#include "EclipsedEngine/Components/Rendering/SpriteRenderer2D.h"

#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"
#include "Renderer/RendererManager.h"

#include "AssetSystem/Assets.h"

#include <OpenGL/glad/glad.h>

namespace Eclipse::Graphics::OpenGL
{
	void OpenGL_InstancedRenderer::Init()
	{
		//myInstanceMaterial = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::MATERIAL_2D_INSTANCE_SPRITE);
	}

	void OpenGL_InstancedRenderer::AddRenderObject(TransformBuffer* aTransformBuffer, SpriteBuffer* aSpriteBuffer, MaterialBuffer* aMaterialBuffer, Texture* aSprite, int PixelPickMaterial)
	{
		Sprite::InstanceTransform transform;
		transform.Position = aTransformBuffer->Position;
		transform.Scale = aTransformBuffer->Scale;
		transform.Rotation = aTransformBuffer->Rotation;
		transform.PixelPickColor = PixelPickMaterial;

		GUID AssetID = {0};

		if (aSprite->IsReady())
			AssetID = aSprite->GetGUID();
		
		InstancedRenderObject& object = RenderComponents[AssetID].emplace_back();
		object.TransformBuffer = aTransformBuffer;
		object.SpriteBuffer = aSpriteBuffer;
		object.MaterialBuffer = aMaterialBuffer;
		object.Sprite = aSprite;
		object.InstanceTransform = transform;
	}

	void OpenGL_InstancedRenderer::ExecuteStaticRendering()
	{
		// Ugla
		static Material myInstanceMaterial = Assets::Load<Material>("engine://Default/Materials/Default_2D_Instance_Material.mat");

		auto* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();
		auto* graphicsDevice = Graphics::RendererManager::GetRenderer().GetDevice();

		for (auto& [AssetID, RenderVector] : RenderComponents)
		{
			if (RenderVector.empty())
				continue;

			InstancedRenderObject& FirstRenderObject = RenderVector.back();

			myInstanceMaterial.Bind();
			if (FirstRenderObject.Sprite->IsReady())
				FirstRenderObject.Sprite->Bind();

			buffer->SetOrCreateBuffer(Graphics::GraphicsBufferEnum::g_SpriteBuffer, 3, FirstRenderObject.SpriteBuffer);
			buffer->SetOrCreateBuffer(Graphics::GraphicsBufferEnum::g_MaterialBuffer, 5, FirstRenderObject.MaterialBuffer);

			std::vector<Sprite::InstanceTransform> instanceTransform;
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
	void OpenGL_InstancedRenderer::ExecutePixelPickingStaticRendering()
	{
		// Ugla
		static Material PixelPickMaterial = Eclipse::AssetSystem::AssetLoader::Load<Material>("engine://Default/Materials/Editor/Default_2D_Instance_PixelPicking_Material.mat");

		auto* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();
		auto* graphicsDevice = Graphics::RendererManager::GetRenderer().GetDevice();

		for (auto& [AssetID, RenderVector] : RenderComponents)
		{
			if (RenderVector.empty())
				continue;

			InstancedRenderObject& FirstRenderObject = RenderVector.back();

			PixelPickMaterial.Bind();
			if (FirstRenderObject.Sprite->IsReady())
				FirstRenderObject.Sprite->Bind();

			buffer->SetOrCreateBuffer(Graphics::GraphicsBufferEnum::g_SpriteBuffer, 3, FirstRenderObject.SpriteBuffer);
			

			std::vector<Sprite::InstanceTransform> instanceTransform;
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