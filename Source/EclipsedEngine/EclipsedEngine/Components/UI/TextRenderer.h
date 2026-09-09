//#pragma once
//
//#include "EclipsedEngine/Components/Rendering/BaseRenderComponent.h"
//#include "EclipsedEngine/Components/Component.h"
//
//#include "Assets/Assets/Shader/VertexShaderAsset.h"
//#include "Assets/Assets/Shader/PixelShaderAsset.h"
//#include "Assets/Assets/FontAsset.h"
//#include "Assets/Assets/MaterialAsset.h"
//
//#include "Core/PathManager.h"
//#include "Core/GraphicsBuffers/TextBuffer.h"
//#include "Core/GraphicsBuffers/TextMaterialBuffer.h"
//#include "Core/GraphicsBuffers/TransformBuffer.h"
//
//#include "EclipsedEngine.Core.hpp"
//
//
//namespace Eclipse
//{
//    class ECL_API TextRenderer : public BaseRenderComponent
//    {
//        COMPONENT_BASE_2(TextRenderer, 20)
//
//    public:
//#ifdef ECL_EDITOR
//        void EditorUpdate() override;
//#endif
//        void OnComponentAdded() override;
//        void TransformUpdate();
//
//        void Render() override;
//        void Draw();
//
//        void SetFont(const char* aFont);
//        void SetFontSize(float aFontSize);
//
//        void SetText(const char* aText);
//
//        //void DrawInspector() override;
//
//        //void DrawMultilineInspector();
//        //void DrawAlignmentInspector(const ImColor& onColor, const ImColor& offColor);
//        //void DrawCenteringInspector(const ImColor& onColor, const ImColor& offColor);
//
//#ifdef ECL_EDITOR
//        void OnDrawGizmos() override;
//#endif
//
//        SERIALIZED_FIELD_DEFAULT(std::string, myText, "TEXT");
//        SERIALIZED_FIELD_DEFAULT(Math::Color, myTextColor, Math::Color(1, 1, 1, 1));
//
//        //SERIALIZED_FIELD_DEFAULT(std::string, myFontPath, (PathManager::GetEngineAssetsPath() / "Fonts/Quicksand-VariableFont_wght.ttf").generic_string());
//        SERIALIZED_FIELD(Assets::Font, font);
//        
//        SERIALIZED_FIELD_STEP_DEFAULT(float, myFontSize, 0.1f, 48.f);
//
//        SERIALIZED_FIELD_STEP_DEFAULT(Math::Vector2f, myRect, 0.1f, Math::Vector2f(1.f, 1.f));
//        SERIALIZED_FIELD_STEP_DEFAULT(float, myCharacterSpacing, 0.1f, 1.f);
//        SERIALIZED_FIELD_STEP_DEFAULT(float, myEnterSpacing, 0.1f, 1.f);
//        SERIALIZED_FIELD_STEP_DEFAULT(float, mySpaceSpacing, 0.1f, 1.f);
//
//        int myLastFontSize = 48;
//
//        static inline bool drawRectGizmos = false;
//
//    private:
//        Assets::Material material;
//
//        PRIVATE_SERIALIZED_FIELD_DEFAULT(int, myTextAlignment, 1);
//        PRIVATE_SERIALIZED_FIELD_DEFAULT(int, myTextCentering, 1);
//
//        std::vector<float> lineOffsets;
//
//        TextMaterialBuffer myTextMaterialBuffer;
//        TransformBuffer myTransformBuffer;
//        TextBuffer myTextBuffer;
//    };
//}
