#include "SpriteRenderer2D.h"

#include "EclipsedEngine/Components/Transform2D.h"
#include "EclipsedEngine/Components/Rendering/Camera.h"
#include "Core/GraphicsBuffers/EditorBuffer.h"

#include "RenderCommands/RenderSprite2DCommand.h"
#include "Renderer/Sprite.h"
#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RenderCommands/CommandList.h"

#include "Physics/PhysicsEngine.h"

namespace Eclipse
{
#ifdef ECLIPSED_NETWORKING
	//void SpriteRenderer2D::sprite_OnRep()
	//{
	//	SetSprite(Sprite->GetAssetID());
	//}
#endif

	void SpriteRenderer2D::SetSpriteRect(const Math::Vector2f& aMin, const Math::Vector2f& aMax)
	{
		spriteRectMin = aMin * Sprite.GetDimDivOne();
		spriteRectMax = aMax * Sprite.GetDimDivOne();
	}

	void SpriteRenderer2D::SetXMirror(bool aMirror)
	{
		mirroredX = aMirror;
	}
	void SpriteRenderer2D::SetYMirror(bool aMirror) { mirroredY = aMirror; }

#pragma region --- Set Sprite
	void SpriteRenderer2D::SetSprite(const Assets::GUID& aGuid)
	{
		assert("No loading assets implemented.");
		//sprite = Resources::Get<Eclipse::Texture>(aGuid);
		hasSprite = true;

		//REPLICATEGARANTIED(sprite);
	}

	void SpriteRenderer2D::SetSprite(const Assets::Texture& aSprite)
	{
		Sprite = aSprite;
		hasSprite = true;

		//REPLICATEGARANTIED(sprite);
	}
#pragma endregion
	void SpriteRenderer2D::SetMaterial(const Assets::GUID& aGuid)
	{
		assert("No loading assets implemented.");
		//material = Resources::Get<Assets::Material>(aGuid);
		hasMaterial = true;
	}

	void SpriteRenderer2D::SetMaterial(const Assets::Material& aMaterial)
	{
		Material = aMaterial;
		hasMaterial = true;
	}


	Assets::Texture SpriteRenderer2D::GetSprite()
	{
		return Sprite;
	}

	void SpriteRenderer2D::OnComponentAdded()
	{
		PixelPickMaterial = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::PIXELPICK_MATERIAL);

		if (Material.IsValid()) hasMaterial = true;
		if (Sprite.IsValid()) hasSprite = true;

		if (!hasMaterial)
		{
			Material = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::MATERIAL_2D_SPRITE);
			hasMaterial = true;
		}
	}

	void SpriteRenderer2D::Render()
	{
		if (!gameObject->transform)
			return;

		Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
		commandListManager->GetSpriteCommandList().Enqueue<RenderSprite2DCommand>(this);
	}

	void SpriteRenderer2D::Draw(unsigned aProgramID)
	{
		if (!hasMaterial || IsDeleted)
			return;

		auto* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();		

		CameraBuffer* cameraBuffer = nullptr;
		buffer->GetBuffer<CameraBuffer>(cameraBuffer);

		Math::Vector2f ScaleWResolutionRatio = 1.f / cameraBuffer->cameraScale;
		ScaleWResolutionRatio.x *= 1.f / cameraBuffer->resolutionRatio;

		Math::Vector2f Min = cameraBuffer->cameraPosition - ScaleWResolutionRatio;
		Math::Vector2f Max = cameraBuffer->cameraPosition + ScaleWResolutionRatio;


		TransformBuffer& transformBuffer = gameObject->transform->myTransformBuffer;

		Math::Vector2f MinBounds = transformBuffer.Position - transformBuffer.Scale * 0.5f * 0.01f;
		Math::Vector2f MaxBounds = transformBuffer.Position + transformBuffer.Scale * 0.5f * 0.01f;

		bool IsInFrustum = Eclipse::PhysicsEngine::InclusiveCollisionCheck(MinBounds, MaxBounds, Min, Max);
		if (!IsInFrustum)
			return;

		DebugDrawer::DrawSquareMinMax(MinBounds, MaxBounds, Math::Color(0.f, 1.f, 0.f, 1.f));

		Graphics::IGraphicsDevice* graphicsDevice = Graphics::RendererManager::GetRenderer().GetDevice();

		Math::Vector2f NewSpriteRectMax = spriteRectMax;
		Math::Vector2f NewSpriteRectMin = spriteRectMin;

		NewSpriteRectMax.y = 1 - NewSpriteRectMax.y;
		NewSpriteRectMin.y = 1 - NewSpriteRectMin.y;

		Math::Vector2f size = NewSpriteRectMax - NewSpriteRectMin;
		Material.GetBuffer().spriteRect = { NewSpriteRectMin.x, NewSpriteRectMin.y, size.x, size.y };

		Math::Vector2f scaleMultiplier;



		mySpriteBuffer.mirrored = { mirroredX ? -1.f : 1.f, mirroredY ? -1.f : 1.f };

#ifdef ECL_EDITOR
		EditorBuffer* editorBuffer;
		buffer->GetBuffer<EditorBuffer>(editorBuffer);

		if (editorBuffer->PixelPicking)
			editorBuffer->PixelPickColor = gameObject->myPixelPickColor;

		buffer->SetOrCreateBuffer<EditorBuffer>(35);

		if (editorBuffer->PixelPicking)
			graphicsDevice->BindMaterial(PixelPickMaterial);
		else
			graphicsDevice->BindMaterial(Material);
#else
		graphicsDevice->BindMaterial(material);
#endif

		if (Sprite.IsValid())
		{
			scaleMultiplier = Sprite.GetTextureSizeNormilized();
			graphicsDevice->BindTexture(0, Sprite);
		}
		else
			scaleMultiplier = Material.GetTexture().GetTextureSizeNormilized();

		buffer->SetOrCreateBuffer(1, transformBuffer);
		buffer->SetOrCreateBuffer(3, mySpriteBuffer);
		buffer->SetOrCreateBuffer(5, Material.GetBuffer());


		Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
		Graphics::IDrawer* drawer = renderer.GetDrawer();
		drawer->DrawSprite();
	}
}
