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

#include <AssetSystem/Loading/AssetLoader.h>

#include <Core/MainSingleton.h>

namespace Eclipse
{
	Material SpriteRenderer2D::PixelPickMaterial;
	bool SpriteRenderer2D::RenderingFrozen = false;

	void SpriteRenderer2D::SetSpriteRect(const Math::Vector2f& aMin, const Math::Vector2f& aMax)
	{
		spriteRectMin = aMin * mySprite.GetDimDivOne();
		spriteRectMax = aMax * mySprite.GetDimDivOne();
	}

	void SpriteRenderer2D::OnChangeColor()
	{
		myMaterialBuffer.color = myMaterial.Color() * myColor;
	}

	void SpriteRenderer2D::OnChangeMaterial()
	{
		hasMaterial = myMaterial.IsReady();

		myMaterialBuffer.color = myMaterial.Color() * myColor;

		Math::Vector2f NewSpriteRectMax = spriteRectMax;
		Math::Vector2f NewSpriteRectMin = spriteRectMin;

		NewSpriteRectMax.y = 1 - NewSpriteRectMax.y;
		NewSpriteRectMin.y = 1 - NewSpriteRectMin.y;

		Math::Vector2f size = NewSpriteRectMax - NewSpriteRectMin;
		myMaterialBuffer.spriteRect = { NewSpriteRectMin.x, NewSpriteRectMin.y, size.x, size.y };
	}

	void SpriteRenderer2D::OnChangeSprite()
	{
		hasSprite = mySprite.IsReady();
	}

	void SpriteRenderer2D::OnChangeMirror()
	{
		mySpriteBuffer.mirrored = { myMirror.x ? -1.f : 1.f, myMirror.y ? -1.f : 1.f };
	}

	void SpriteRenderer2D::OnChangeZIndex()
	{
		InternalzIndex = zIndex;
	}

	void SpriteRenderer2D::SetSprite(const GUID& aGuid)
	{
		if (!aGuid.IsEmpty())
		{
			hasSprite = false;
			return;
		}

		
		SetSprite(Eclipse::AssetSystem::AssetLoader::Load<Texture>(aGuid));
	}

	void SpriteRenderer2D::SetSprite(const Texture& aSprite)
	{
		mySprite = aSprite;
		
		if (mySprite.IsReady()) OnChangeSprite();
	}


	void SpriteRenderer2D::SetMaterial(const GUID& aGuid)
	{
		if (!aGuid.IsEmpty())
		{
			hasMaterial = false;
			return;
		}

		SetSprite(Eclipse::AssetSystem::AssetLoader::Load<Texture>(aGuid));
	}

	void SpriteRenderer2D::SetMaterial(const Material& aMaterial)
	{
		myMaterial = aMaterial;

		if (myMaterial.IsReady()) OnChangeMaterial();
	}

	void SpriteRenderer2D::SetColor(const Math::Color& aColor)
	{
		myColor = aColor;
		OnChangeColor();
	}

	void SpriteRenderer2D::OnComponentAdded()
	{
		PixelPickMaterial = Eclipse::AssetSystem::AssetLoader::Load<Material>("engine://Default/Materials/Editor/PixelPick_Material.mat");

		if (myMaterial.IsReady()) hasMaterial = true;
		if (mySprite.IsReady()) hasSprite = true;

		if (!hasMaterial)
		{
			myMaterial = Eclipse::AssetSystem::AssetLoader::Load<Material>("engine://Default/Materials/Default_2D_Material.mat");
			hasMaterial = true;
		}

		renderer = &Graphics::RendererManager::GetRenderer();
		drawer = renderer->GetDrawer();

		Eclipse::Graphics::IGraphicsBuffer* buffer = renderer->GetGraphicsBuffer();
		buffer->GetBuffer(Graphics::GraphicsBufferEnum::g_EditorBuffer, editorBuffer);

		graphicsDevice = Graphics::RendererManager::GetRenderer().GetDevice();
	}

	void SpriteRenderer2D::CheckInFrustum()
	{
		auto* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();
		CameraBuffer* cameraBuffer = nullptr;
		buffer->GetBuffer(Graphics::GraphicsBufferEnum::g_CameraBuffer, cameraBuffer);

		Math::Vector2f ScaleWResolutionRatio = 1.f / cameraBuffer->cameraScale;
		ScaleWResolutionRatio.x *= 1.f / cameraBuffer->resolutionRatio;

		Math::Vector2f Min = cameraBuffer->cameraPosition - ScaleWResolutionRatio;
		Math::Vector2f Max = cameraBuffer->cameraPosition + ScaleWResolutionRatio;

		TransformBuffer& transformBuffer = gameObject->transform->myTransformBuffer;

		Math::Vector2f AbsScale(std::abs(transformBuffer.Scale.x), std::abs(transformBuffer.Scale.y));

		Math::Vector2f HalfExtents = AbsScale * 0.5f * 0.01f;

		float Radians = transformBuffer.Rotation;
		float C = std::abs(std::cos(Radians));
		float S = std::abs(std::sin(Radians));

		Math::Vector2f RotatedExtents;
		RotatedExtents.x = HalfExtents.x * C + HalfExtents.y * S;
		RotatedExtents.y = HalfExtents.x * S + HalfExtents.y * C;

		Math::Vector2f MinBounds = transformBuffer.Position - RotatedExtents;
		Math::Vector2f MaxBounds = transformBuffer.Position + RotatedExtents;

		IsInFrustum = Eclipse::PhysicsEngine::InclusiveCollisionCheck(MinBounds, MaxBounds, Min, Max);
	}

	void SpriteRenderer2D::Render()
	{
		if (!gameObject->transform)
			return;

		Graphics::CommandListManager* commandListManager = Graphics::RendererManager::GetRenderer().GetCommandListManager();
		commandListManager->GetSpriteCommandList().Enqueue<RenderSprite2DCommand>(this);
	}

	void SpriteRenderer2D::Draw(unsigned aProgramID)
	{
		if (!hasMaterial || IsDeleted)
			return;

		if (!RenderingFrozen)
			CheckInFrustum();

		if (!IsInFrustum)
			return;

		if (gameObject->GetIsStatic())
		{
			Graphics::IInstancedRenderer* InstancedRenderer = Graphics::RendererManager::GetRenderer().GetInstancedRendering();
			InstancedRenderer->AddRenderObject(&gameObject->transform->myTransformBuffer, &mySpriteBuffer, &myMaterialBuffer, &mySprite);

			return;
		}
		

		auto* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();

#ifdef ECL_EDITOR
		if (editorBuffer->PixelPicking)
			editorBuffer->PixelPickColor = gameObject->myPixelPickColor;

		buffer->SetOrCreateBuffer(Graphics::GraphicsBufferEnum::g_EditorBuffer, 35);

		if (editorBuffer->PixelPicking)
			PixelPickMaterial.Bind();
		else
			myMaterial.Bind();
#else
		graphicsDevice->BindMaterial(material);
#endif

		//Math::Vector2f scaleMultiplier;
		//if (mySprite.IsValid())
		//{
		//	//scaleMultiplier = mySprite.GetTextureSizeNormilized();
		//	graphicsDevice->BindTexture(0, mySprite);
		//}
		//else
		//	scaleMultiplier = myMaterial.GetTexture().GetTextureSizeNormilized();

		buffer->SetOrCreateBuffer(Graphics::GraphicsBufferEnum::g_TransformBuffer, 1, &gameObject->transform->myTransformBuffer);
		buffer->SetOrCreateBuffer(Graphics::GraphicsBufferEnum::g_SpriteBuffer, 3, &mySpriteBuffer);
		buffer->SetOrCreateBuffer(Graphics::GraphicsBufferEnum::g_MaterialBuffer, 5, &myMaterialBuffer);

		drawer->DrawSprite();
	}
}
