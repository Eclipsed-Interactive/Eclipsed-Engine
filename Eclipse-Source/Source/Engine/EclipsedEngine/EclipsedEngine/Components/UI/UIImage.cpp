#include "UIImage.h"

#include "GraphicsEngine/Sprite.h"

#include "AssetEngine/Resources.h"
#include "CoreEngine/Settings/GraphicsSettings.h"

#include "EclipsedEngine/Components/UI/RectTransform.h"
#include "EclipsedEngine/Components/UI/Canvas.h"

#include "GraphicsEngine/RenderCommands/CommandList.h"

#include "GraphicsEngine/OpenGL/OpenGLGraphicsAPI.h"

namespace Eclipse
{
    void UIImage::sprite_OnRep()
    {
    }

    void UIImage::EditorUpdate()
    {
        auto transform = gameObject->GetComponent<RectTransform>();

        if (transform->myCanvas)
        {
            float sizeX = (transform->WidthHeightPX->x / transform->myCanvas->ReferenceResolution->y) * 0.5f;
            float sizeY = (transform->WidthHeightPX->y / transform->myCanvas->ReferenceResolution->y) * 0.5f;

            float posX = transform->Position->x / transform->myCanvas->ReferenceResolution->y + 0.5f;
            float posY = transform->Position->y / transform->myCanvas->ReferenceResolution->y + 0.5f;

            Math::Vector2f sqrPos = {posX, posY};
            Math::Vector2f sqrSize = {sizeX, sizeY};

            DebugDrawer::DrawSquare(sqrPos, 0, sqrSize, Math::Color(0x90D5FF));
        }
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
        CommandListManager::GetUICommandList().Enqueue([&]()
        {
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
        Math::Vector2f referenceResolution = canvas->ReferenceResolution;
        Math::Vector2f halfRefRes = referenceResolution * 0.5f;

#ifdef ECLIPSED_EDITOR
        Math::Vector2f resolution = Editor::GameWindow::myGameImageResolution;
#endif

#ifndef ECLIPSED_EDITOR
        Math::Vector2f resolution = Settings::GraphicsSettings::GetResolution();
#endif

        Math::Vector2f resMinRef = resolution - referenceResolution;

        Math::Vector2f halfRes = resolution * 0.5f;
        Math::Vector2f position = tranform->GetPosition();

        myTransformBuffer.Position = position;

        if (IsScene)
            myTransformBuffer.Position *= canvasCameraTransform.ScaleMultiplier;
        else
            myTransformBuffer.Position *= Math::Vector2f(2, 2);

        myTransformBuffer.Position += canvasCameraTransform.PositionOffset;

        // if (!IsScene)
        // {
        //     myTransformBuffer.Position.x += tranform->WidthHeightPX->x * 0.5f;
        // }

        Math::Vector2f WidthHeightPX = tranform->WidthHeightPX.Get();
        myTransformBuffer.Scale = WidthHeightPX;

        Math::Vector2f canvasScaleRelationOneDiv = {1.f / referenceResolution.x, 1.f / referenceResolution.y};
        myTransformBuffer.Scale *= canvasScaleRelationOneDiv;

        Math::Vector2f multiplier;
        if (IsScene)
            multiplier = canvasCameraTransform.ScaleMultiplier;
        else
        {
            if (!tranform->ScaleWithCanvasX)
                multiplier.x = canvasCameraTransform.ScaleMultiplier.x;
            else
                multiplier.x = 2;
            if (!tranform->ScaleWithCanvasY)
                multiplier.y = canvasCameraTransform.ScaleMultiplier.y;
            else
                multiplier.y = 2;
        }
        myTransformBuffer.Scale *= multiplier;

        myTransformBuffer.Rotation = canvasCameraTransform.Rotation;
    }

    void UIImage::Draw()
    {
        
        if (!hasMaterial)
            return;


        auto transform = gameObject->GetComponent<RectTransform>();
        if (!transform)
            return;
        if (!transform->myCanvas)
            return;

        transform->myCanvas->SetCanvasTransformProperties();


        Math::Vector2f resolution = transform->myCanvas->ReferenceResolution;

        resolution.x = 1.f / resolution.x;
        resolution.y = 1.f / resolution.y;

        // float aspectRatio = resolution.y / resolution.x;
        // Math::Vector2f canvasScaleRelationOneDiv = {resolution.x, resolution.y};

        Math::Vector2f size = spriteRectMax - spriteRectMin;
        material->myMaterialBuffer.spriteRect = {spriteRectMin.x, spriteRectMin.y, size.x, size.y};

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

        CanvasBuffer* canvasBuffer;
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->GetBuffer<CanvasBuffer>(canvasBuffer);
        if (!IsScene)
        {
            if (!transform->myCanvas->WorldSpace)
                canvasBuffer->canvasPositionOffset = transform->myCanvas->canvasCameraTransform.PositionOffset;
            else
                canvasBuffer->canvasPositionOffset = {0, 0};
            
        }
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(2, *canvasBuffer);

        Sprite::Get().Render();
    }
}
