#pragma once

#include "EclipsedEngine/Components/Component.h"
#include "EclipsedEngine/Components/Rendering/SpriteRenderer2D.h"

//#include "AssetEngine/Assets/SpriteAnimation.h"

namespace Eclipse
{
    class SpriteRenderer2D;

    class ECLIPSED_API SpriteSheetAnimator2D : public Component
    {
        BASE_SELECTION(SpriteSheetAnimator2D, 10)

    public:
        void OnComponentAdded() override;

        void Update() override;

        void SetSpriteSheet(const char* aPath);

        void Play() { myIsPlaying = true; }
        void Pause() { myIsPlaying = false; }

        void SetIsLooping(bool IsLooping) { myLoop = IsLooping; }

        void SetCurrentAnimation(const char* anAnimationName, bool aLoop = true);

    private:

        //SERIALIZED_FIELD_DEFAULT(std::string, mySpriteSheetAnimationPath, "");

    private:
        std::vector<unsigned> TemporarySpriteAnimation{ 0,1,2,3,4,5,6,7,0 };

        SpriteRenderer2D* mySpriteRenderer;
        std::string myActiveAnimation;

        SERIALIZED_FIELD_DEFAULT(float, myTimePerFrame, 0.04f);
        float myTimeAccumulator = 0.f;

        REPLICATED_PRIVATE_SERIALIZED_FIELD_DEFAULT(int, myCurrentFrame, 0, SpriteSheetAnimator2D);
        SERIALIZED_FIELD_DEFAULT(bool, myLoop, true);

        bool myIsPlaying = false;
    };
}