#include "PhysicsEngine.h"

#include "box2d/box2d.h"

namespace Eclipse
{
    bool PhysicsEngine::InclusiveCollisionCheck(Math::Vector2f MinBoundsSelf, Math::Vector2f MaxBoundsSelf, Math::Vector2f MinBoundsOther, Math::Vector2f MaxBoundsOther)
    {
        if (MinBoundsOther.y > MaxBoundsSelf.y)
            return false;
        if (MaxBoundsOther.y < MinBoundsSelf.y)
            return false;

        if (MaxBoundsOther.x < MinBoundsSelf.x)
            return false;
        if (MinBoundsOther.x > MaxBoundsSelf.x)
            return false;

        return true;
    }
}
