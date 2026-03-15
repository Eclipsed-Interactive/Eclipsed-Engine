#include "SpriteRenderer2D.h"

#include "EclipsedEngine/Components/Transform2D.h"
#include "GraphicsEngine/Sprite.h"

#include "GraphicsEngine/OpenGL/OpenGLGraphicsAPI.h"
#include "GraphicsEngine/RenderCommands/CommandList.h"

#include "RenderCommands/RenderSprite2DCommand.h"

#include "OpenGL/glad/glad.h"

#include "AssetEngine/Resources.h"

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
			material = Resources::GetDefault<Material>(Sprite);
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
		if (!hasMaterial)
			return;

		Math::Vector2f position = gameObject->transform->GetPosition();
		float rotation = gameObject->transform->GetRotation();
		Math::Vector2f scale = gameObject->transform->GetScale();

		unsigned shaderID = material->GetShaderProgramID();

		if (aProgramID)
			shaderID = aProgramID;

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


		GraphicsEngine::SetUniform(UniformType::Vector2f, shaderID, "transform.position", &position);
		GraphicsEngine::SetUniform(UniformType::Float, shaderID, "transform.rotation", &rotation);
		GraphicsEngine::SetUniform(UniformType::Vector2f, shaderID, "transform.size", &scale);

		Math::Vector2f size = spriteRectMax - spriteRectMin;
		Math::Vector4f spriteRect = { spriteRectMin.x, spriteRectMin.y, size.x, size.y };
		GraphicsEngine::SetUniform(UniformType::Vector4f, shaderID, "material.spriteRect", &spriteRect);

		Math::Vector2f scaleMultiplier;
		if (sprite->IsValid())
			scaleMultiplier = sprite->GetTextureSizeNormilized();
		else
			scaleMultiplier = material->GetTexture().GetTextureSizeNormilized();

		float aspectScale = size.y / size.x;
		Math::Vector2f spriteScaleMultiplier = { scaleMultiplier.x, scaleMultiplier.y * aspectScale };
		GraphicsEngine::SetUniform(UniformType::Vector2f, shaderID, "spriteScaleMultiplier", &spriteScaleMultiplier);

		Math::Vector2f mirroredVec2 = { mirroredX ? -1.f : 1.f, mirroredY ? -1.f : 1.f };
		GraphicsEngine::SetUniform(UniformType::Vector2f, shaderID, "mirrored", &mirroredVec2);

		Math::Vector4f pixelPickColor = gameObject->GetPixelPickingIDColor();
		GraphicsEngine::SetUniform(UniformType::Vector4f, shaderID, "pixelPickColor", &pixelPickColor);

		GraphicsEngine::SetGlobalUniforms(shaderID);

		Sprite::Get().Render();
	}
}