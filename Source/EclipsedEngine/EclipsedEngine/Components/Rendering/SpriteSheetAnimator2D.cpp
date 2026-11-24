#include "SpriteSheetAnimator2D.h"

#include "Components/Rendering/SpriteRenderer2D.h"

#include "ECS/ComponentManager.h"
#include "Core/Timer.h"

namespace Eclipse
{
#ifdef ECLIPSED_NETWORKING
   void SpriteSheetAnimator2D::myCurrentFrame_OnRep()
   {
       //const std::vector<unsigned>& animationFramesIdx = TemporarySpriteAnimation;
       //if (animationFramesIdx.empty())
       //    return;
       //if (!mySpriteRenderer)
       //    return;

       //Eclipse::AssetSystem::Texture sprite = mySpriteRenderer->GetSprite();
       //if (!sprite.IsReady())
       //    return;

       //const std::vector<Math::RectSizePos>& spriteRects = *sprite.GetSpriteRects();
       //if (spriteRects.empty())
       //    return;

       //const Math::RectSizePos& rect = spriteRects[animationFramesIdx[myCurrentFrame]];
       //mySpriteRenderer->SetSpriteRect(rect.position, rect.position + rect.size);
   }
#endif

   void SpriteSheetAnimator2D::Update()
   {
       if (!myIsOwner)
           return;

       if (!myIsPlaying || !mySpriteRenderer)
           return;

       //myTimeAccumulator += Time::GetDeltaTime();

       if (myTimeAccumulator >= myTimePerFrame)
       {
           //const std::vector<unsigned>& animationFramesIdx = mySpriteSheetAnimations->GetAnimation(myActiveAnimation);
           const std::vector<unsigned>& animationFramesIdx = TemporarySpriteAnimation;
           if (animationFramesIdx.empty())
               return;

           myTimeAccumulator -= myTimePerFrame;

           if (myCurrentFrame == animationFramesIdx.size() - 1)
           {
               myCurrentFrame = 0;

               if (!myLoop)
               {
                   myIsPlaying = false;
                   return;
               }
           }

           const std::vector<Math::RectSizePos>& spriteRects = *mySpriteRenderer->GetSprite().GetSpriteRects();

           if (spriteRects.empty())
               return;

           const Math::RectSizePos& rect = spriteRects[animationFramesIdx[myCurrentFrame++]];
           mySpriteRenderer->SetSpriteRect(rect.position, rect.position + rect.size);
       }
   }

   void SpriteSheetAnimator2D::OnComponentAdded()
   {
       mySpriteRenderer = gameObject->GetComponent<SpriteRenderer2D>();
       if (!mySpriteRenderer)
           return;

       Texture sprite = mySpriteRenderer->GetSprite();
       if (!sprite.IsReady())
           return;

       const std::vector<Math::RectSizePos>& spriteRects = *sprite.GetSpriteRects();
       if (spriteRects.empty())
           return;

       const Math::RectSizePos& rect = spriteRects[0];
       mySpriteRenderer->SetSpriteRect(rect.position, rect.position + rect.size);
   }

   void SpriteSheetAnimator2D::SetSpriteSheet(const char* aPath)
   {
       //mySpriteSheetAnimations = Resources::Get<SpriteSheetAnimation>(aPath);

       //mySpriteSheet.Load(aPath);
       myCurrentFrame = 0;
       myTimeAccumulator = 0.f;
   }

   void SpriteSheetAnimator2D::SetCurrentAnimation(const char* anAnimationName, bool aLoop)
   {
       myActiveAnimation = anAnimationName;

       myLoop = aLoop;
       myIsPlaying = true;
   }

//#ifdef ECL_EDITOR
//   void SpriteSheetAnimator2D::SetFirstSpriteSheet()
//   {
//       if (isUsingspritesheet)
//       {
//           isUsingspritesheet = false;
//
//           mySpriteRenderer->spriteRectMin = { 0, 0 };
//           mySpriteRenderer->spriteRectMax = {1, 1};
//       }
//       else
//       {
//           isUsingspritesheet = true;
//
//           Assets::Texture sprite = mySpriteRenderer->GetSprite();
//           if (!sprite.IsValid())
//               return;
//
//           const std::vector<Math::RectSizePos>& spriteRects = sprite.GetSpriteRects();
//           if (spriteRects.empty())
//               return;
//
//           const Math::RectSizePos& rect = spriteRects[0];
//           mySpriteRenderer->SetSpriteRect(rect.position, rect.position + rect.size);
//       }
//   }
//#endif
}