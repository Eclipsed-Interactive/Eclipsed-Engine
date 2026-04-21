#include "SpriteRenderer2D.h"

#include "EclipsedEngine/Components/Transform2D.h"
#include "GraphicsEngine/Sprite.h"

#include "GraphicsEngine/OpenGL/OpenGLGraphicsAPI.h"
#include "GraphicsEngine/RenderCommands/CommandList.h"

#include "RenderCommands/RenderSprite2DCommand.h"

#include "OpenGL/glad/glad.h"

#include "AssetEngine/Resources.h"
#include "CoreEngine/GraphicsBuffers/EditorBuffer.h"

namespace Eclipse
{
	void SpriteRenderer2D::sprite_OnRep()
	{
		SetSprite(sprite->GetAssetID());
	}

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
	void SpriteRenderer2D::SetSprite(const std::string& aGuid)
	{
		sprite = Resources::Get<Eclipse::Texture>(aGuid);
		hasSprite = true;

		REPLICATEGARANTIED(sprite);
	}

	void SpriteRenderer2D::SetSprite(const Eclipse::Texture& aSprite)
	{
		sprite = aSprite;
		hasSprite = true;

		REPLICATEGARANTIED(sprite);
	}
#pragma endregion
	void SpriteRenderer2D::SetMaterial(const std::string& aGuid)
	{
		material = Resources::Get<Eclipse::Material>(aGuid);
		hasMaterial = true;
	}

	void SpriteRenderer2D::SetMaterial(const Eclipse::Material& aMaterial)
	{
		material = aMaterial;
		hasMaterial = true;
	}


	Texture SpriteRenderer2D::GetSprite()
	{
		return sprite;
	}

	void SpriteRenderer2D::OnComponentAdded()
	{
		if (material->IsValid()) hasMaterial = true;
		if (sprite->IsValid()) hasSprite = true;

		if (!hasMaterial)
		{
			material = Resources::GetDefaultSpriteMaterial();
			material->Create();

			hasMaterial = true;
		}
	}

	void SpriteRenderer2D::Render()
	{
		if (!gameObject->transform)
			return;

		CommandListManager::GetSpriteCommandList().Enqueue<RenderSprite2DCommand>(this);
		//DebugInformationCollector::UpdateRenderCalls();
	}

	void SpriteRenderer2D::Draw(unsigned aProgramID)
	{
		if (!hasMaterial || IsDeleted)
			return;

		// unsigned shaderID = material->GetShaderProgramID();
		//
		// if (aProgramID)
		// 	shaderID = aProgramID;

		if (sprite->IsValid())
		{
			material->BindShader();
			sprite->Bind();
			material->BindColor();
		}
		else
		{
			material->Use();
		}
		
		myTransformBuffer.Position = gameObject->transform->GetPosition();
		myTransformBuffer.Rotation = gameObject->transform->GetRotation();
		myTransformBuffer.Scale = gameObject->transform->GetScale();

		Math::Vector2f size = spriteRectMax - spriteRectMin;
		material->myMaterialBuffer.spriteRect = { spriteRectMin.x, spriteRectMin.y, size.x, size.y };

		Math::Vector2f scaleMultiplier;
		if (sprite->IsValid())
			scaleMultiplier = sprite->GetTextureSizeNormilized();
		else
			scaleMultiplier = material->GetTexture().GetTextureSizeNormilized();

		float aspectScale = size.y / size.x;
		mySpriteBuffer.spriteScaleMultiplier = { scaleMultiplier.x, scaleMultiplier.y * aspectScale };
		mySpriteBuffer.mirrored = { mirroredX ? -1.f : 1.f, mirroredY ? -1.f : 1.f };

#ifdef ECLIPSED_EDITOR
		EditorBuffer* editorBuffer;
		GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->GetBuffer<EditorBuffer>(editorBuffer);
		editorBuffer->PixelPickColor = gameObject->GetPixelPickingIDColor();
		GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer<EditorBuffer>(35);
#endif
		GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(5, material->myMaterialBuffer);
		
		GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(1, myTransformBuffer);
		GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(3, mySpriteBuffer);

		Sprite::Get().Render();
	}
}
