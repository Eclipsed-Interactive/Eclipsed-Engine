#include "UIImage.h"

#include "GraphicsEngine/Sprite.h"

#include "AssetEngine/Resources.h"

#include "EclipsedEngine/Components/UI/RectTransform.h"
#include "EclipsedEngine/Components/UI/Canvas.h"

#include "GraphicsEngine/RenderCommands/CommandList.h"

#include "GraphicsEngine/OpenGL/OpenGLGraphicsAPI.h"

namespace Eclipse
{
    void UIImage::sprite_OnRep()
    {
    }

    void UIImage::OnSceneLoaded()
    {
        
    }

    void UIImage::OnComponentAdded()
    {
        if (material->IsValid()) hasMaterial = true;
        if (sprite->IsValid()) hasSprite = true;

        if (!hasMaterial)
        {
            material = Resources::GetDefault<Material>();
            material->Create();

            hasMaterial = true;
        }
    }

    void UIImage::Render()
    {
        CommandListManager::GetUICommandList().Enqueue([&]() {
            Draw();
        });
    }

    void UIImage::Draw()
    {
        if (!hasMaterial)
            return;

        auto tranform = gameObject->GetComponent<RectTransform>();

        if (!tranform)
            return;
        if (!tranform->myCanvas)
            return;

        tranform->myCanvas->SetCanvasTransformProperties();

        Canvas::EditorCanvasCameraTransform& canvasCameraTransform = tranform->myCanvas->canvasCameraTransform;


        Math::Vector2f resolution = tranform->myCanvas->ReferenceResolution;

        resolution.x = 1.f / resolution.x;
        resolution.y = 1.f / resolution.y;

        float aspectRatio = resolution.y / resolution.x;

        Math::Vector2f canvasScaleRelationOneDiv = {resolution.x, resolution.y};

        Math::Vector2f position = tranform->Position;
        position *= canvasScaleRelationOneDiv;
        position *= canvasCameraTransform.ScaleMultiplier;
        position.x *= aspectRatio;

        position += canvasCameraTransform.PositionOffset;

        Math::Vector2f scale = tranform->WidthHeightPX;
        scale *= canvasScaleRelationOneDiv;
        scale *= Math::Vector2f(200, 200);
        scale.x *= aspectRatio;
        scale *= canvasCameraTransform.ScaleMultiplier;

        float rotation = 0.f;
        rotation += canvasCameraTransform.Rotation;

        unsigned shaderID = material->GetShaderProgramID();

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

        //GraphicsEngine::SetUniform(UniformType::Int, shaderID, "ZIndex", &ZIndex.Get());

        GraphicsEngine::SetUniform(UniformType::Vector2f, shaderID, "transform.position", &position);
        GraphicsEngine::SetUniform(UniformType::Float, shaderID, "transform.rotation", &rotation);
        GraphicsEngine::SetUniform(UniformType::Vector2f, shaderID, "transform.size", &scale);

        //auto tempSettings = TemporarySettingsSingleton::Get();

        GraphicsEngine::SetUniform(UniformType::Vector2f, shaderID, "canvasScaleRelationOneDiv", &canvasScaleRelationOneDiv);

        // Math::Vector4f pixelPickColor = gameObject->GetPixelPickingIDColor();
        // GraphicsEngine::SetUniform(UniformType::Vector4f, shaderID, "pixelPickColor", &pixelPickColor);

        GraphicsEngine::SetGlobalUniforms(shaderID);

        Sprite::Get().Render();
    }
}
