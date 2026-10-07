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

namespace Eclipse
{
#ifdef ECLIPSED_NETWORKING
	void SpriteRenderer2D::sprite_OnRep()
	{
		SetSprite(sprite->GetAssetID());
	}
#endif

	void SpriteRenderer2D::SetSpriteRect(const Math::Vector2f& aMin, const Math::Vector2f& aMax)
	{
		spriteRectMin = aMin * sprite->GetDimDivOne();
		spriteRectMax = aMax * sprite->GetDimDivOne();
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
		sprite = aSprite;
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
		material = aMaterial;
		hasMaterial = true;
	}


	Assets::Texture SpriteRenderer2D::GetSprite()
	{
		return sprite;
	}

	void SpriteRenderer2D::OnComponentAdded()
	{
		PixelPickMaterial = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::PIXELPICK_MATERIAL);

		if (material->IsValid()) hasMaterial = true;
		if (sprite->IsValid()) hasSprite = true;

		if (!hasMaterial)
		{
			material = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::MATERIAL_2D_SPRITE);
			hasMaterial = true;
		}
	}

	bool InclusiveCollisionCheck(Math::Vector2f MinBoundsSelf, Math::Vector2f MaxBoundsSelf, Math::Vector2f MinBoundsOther, Math::Vector2f MaxBoundsOther)
	{
		if (MaxBoundsOther.y < MaxBoundsSelf.y)
			return false;
		if (MinBoundsOther.y > MinBoundsSelf.y)
			return false;

		if (MaxBoundsOther.x < MinBoundsSelf.x)
			return false;
		if (MinBoundsOther.x > MaxBoundsSelf.x)
			return false;

		return true;
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

		Math::Vector2f MinBounds = myTransformBuffer.Position - myTransformBuffer.Scale * 0.5f * 0.01f;
		Math::Vector2f MaxBounds = myTransformBuffer.Position + myTransformBuffer.Scale * 0.5f * 0.01f;

		DebugDrawer::DrawSquareMinMax(MinBounds, MaxBounds, Math::Color(0.f, 1.f, 0.f, 1.f));
		

		CameraBuffer* cameraBuffer = nullptr;
		buffer->GetBuffer<CameraBuffer>(cameraBuffer);

		Math::Vector2f ScaleWResolutionRatio = 1.f / cameraBuffer->cameraScale;
		ScaleWResolutionRatio.x *= 1.f / cameraBuffer->resolutionRatio;

		Math::Vector2f Min = cameraBuffer->cameraPosition - ScaleWResolutionRatio;
		Math::Vector2f Max = cameraBuffer->cameraPosition + ScaleWResolutionRatio;

		bool IsInFrustum = InclusiveCollisionCheck(MinBounds, MaxBounds, Min, Max);
		if (!IsInFrustum)
		{
			DebugDrawer::DrawSquareMinMax({ 0, 0 }, { 0.1f, 0.1f }, Math::Color(0.f, 1.f, 0.f, 1.f));
			return;
		}

		Graphics::IGraphicsDevice* graphicsDevice = Graphics::RendererManager::GetRenderer().GetDevice();

		myTransformBuffer.Position = gameObject->transform->GetPosition();
		myTransformBuffer.Rotation = gameObject->transform->GetRotation();
		myTransformBuffer.Scale = gameObject->transform->GetScale();

		Math::Vector2f NewSpriteRectMax = spriteRectMax;
		Math::Vector2f NewSpriteRectMin = spriteRectMin;

		NewSpriteRectMax.y = 1 - NewSpriteRectMax.y;
		NewSpriteRectMin.y = 1 - NewSpriteRectMin.y;

		Math::Vector2f size = NewSpriteRectMax - NewSpriteRectMin;
		material->GetBuffer().spriteRect = { NewSpriteRectMin.x, NewSpriteRectMin.y, size.x, size.y };

		Math::Vector2f scaleMultiplier;
		if (sprite->IsValid())
		{
			scaleMultiplier = sprite->GetTextureSizeNormilized();
			graphicsDevice->BindTexture(0, sprite);
		}
		else
			scaleMultiplier = material->GetTexture().GetTextureSizeNormilized();


		float aspectScale = size.y / size.x;
		mySpriteBuffer.spriteScaleMultiplier = { scaleMultiplier.x, scaleMultiplier.y * aspectScale };
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
#endif
			graphicsDevice->BindMaterial(material);


		buffer->SetOrCreateBuffer(5, material->GetBuffer());

		buffer->SetOrCreateBuffer(1, myTransformBuffer);
		buffer->SetOrCreateBuffer(3, mySpriteBuffer);

		Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
		Graphics::IDrawer* drawer = renderer.GetDrawer();
		drawer->DrawSprite();
	}
}
