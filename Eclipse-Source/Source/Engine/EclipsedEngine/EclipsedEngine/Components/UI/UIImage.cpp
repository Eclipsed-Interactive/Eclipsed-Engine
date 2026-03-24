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

    void UIImage::OnComponentAdded()
    {
        if (material->IsValid()) hasMaterial = true;
        if (sprite->IsValid()) hasSprite = true;

        if (!hasMaterial)
        {
            material = Resources::GetDefault<Material>(UI);
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

    void UIImage::TransformUpdate()
    {
        auto tranform = gameObject->GetComponent<RectTransform>();

        Canvas* canvas = tranform->myCanvas;
        if (!canvas)
        {
            tranform->myIsDirty = false;
            return;
        }
		
        Canvas::EditorCanvasCameraTransform& canvasCameraTransform = canvas->canvasCameraTransform;
        Math::Vector2f resolution = canvas->ReferenceResolution;
        Math::Vector2f canvasScaleRelationOneDiv = { 1.f / resolution.x, 1.f / resolution.y };
		
        myTransformBuffer.Position = tranform->GetPosition();
        myTransformBuffer.Position *= canvasCameraTransform.ScaleMultiplier;
        myTransformBuffer.Position += canvasCameraTransform.PositionOffset;
		
        myTransformBuffer.Scale = (*(Math::Vector2f*)tranform->WidthHeightPX.GetData() * canvasScaleRelationOneDiv);
        myTransformBuffer.Scale *= canvasCameraTransform.ScaleMultiplier;
		
        myTransformBuffer.Rotation = canvasCameraTransform.Rotation;
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


        Math::Vector2f resolution = tranform->myCanvas->ReferenceResolution;

        resolution.x = 1.f / resolution.x;
        resolution.y = 1.f / resolution.y;

        float aspectRatio = resolution.y / resolution.x;

        Math::Vector2f canvasScaleRelationOneDiv = {resolution.x, resolution.y};

        Math::Vector2f size = spriteRectMax - spriteRectMin;
        material->myMaterialBuffer.spriteRect = { spriteRectMin.x, spriteRectMin.y, size.x, size.y };

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

        TransformUpdate();

#ifdef ECLIPSED_EDITOR
        EditorBuffer* editorBuffer;
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->GetBuffer<EditorBuffer>(editorBuffer);
        editorBuffer->PixelPickColor = gameObject->GetPixelPickingIDColor();
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer<EditorBuffer>(35);
#endif
        
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(5, material->myMaterialBuffer);
        
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(1, myTransformBuffer);

        Sprite::Get().Render();
    }
}
