#include "SpriteRenderer2D.h"

#include "EclipsedEngine/Components/Transform2D.h"
#include "Renderer/Sprite.h"

#include "Renderer/IRenderer.h"
#include "Renderer/RenderCommands/CommandList.h"

#include "RenderCommands/RenderSprite2DCommand.h"

#include "OpenGL/glad/glad.h"

#include "Core/GraphicsBuffers/EditorBuffer.h"

#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"
#include "Renderer/RendererManager.h"
#include "RenderCommands/RenderSprite2DCommand.h"
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
		if (material->IsValid()) hasMaterial = true;
		if (sprite->IsValid()) hasSprite = true;

		if (!hasMaterial)
		{
			material = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::MATERIAL_2D_SPRITE);
			hasMaterial = true;
		}
	}

	void SpriteRenderer2D::Render()
	{
		if (!gameObject->transform)
			return;

		Graphics::CommandListManager::GetSpriteCommandList().Enqueue<RenderSprite2DCommand>(this);
	}

	void SpriteRenderer2D::Draw(unsigned aProgramID)
	{
		if (!hasMaterial || IsDeleted)
			return;

		// unsigned shaderID = material->GetShaderProgramID();
		//
		// if (aProgramID)
		// 	shaderID = aProgramID;

		Graphics::IGraphicsDevice* graphicsDevice = Graphics::RendererManager::GetRenderer().GetDevice();
		graphicsDevice->BindMaterial(material);
		if (sprite->IsValid())
			graphicsDevice->BindTexture(0, sprite);

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
			scaleMultiplier = sprite->GetTextureSizeNormilized();
		else
			scaleMultiplier = material->GetTexture().GetTextureSizeNormilized();


		float aspectScale = size.y / size.x;
		mySpriteBuffer.spriteScaleMultiplier = { scaleMultiplier.x, scaleMultiplier.y * aspectScale };
		mySpriteBuffer.mirrored = { mirroredX ? -1.f : 1.f, mirroredY ? -1.f : 1.f };

		Graphics::IGraphicsBuffer* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();

#ifdef ECL_EDITOR
		EditorBuffer* editorBuffer;
		buffer->GetBuffer<EditorBuffer>(editorBuffer);
		editorBuffer->PixelPickColor = gameObject->myPixelPickColor;
		buffer->SetOrCreateBuffer<EditorBuffer>(35);
#endif
		buffer->SetOrCreateBuffer(5, material->GetBuffer());

		buffer->SetOrCreateBuffer(1, myTransformBuffer);
		buffer->SetOrCreateBuffer(3, mySpriteBuffer);


		Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
		Graphics::IDrawer* drawer = renderer.GetDrawer();
		drawer->DrawSprite();
	}
}
