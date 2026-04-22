#include "PhysicsEngine.h"

namespace Eclipse
{
    void Physics::SetLinearVelocity(const b2BodyId* aBodyID, const Math::Vector2f& aVelocity)
    {
        if (!aBodyID)
            return;
        
        b2Vec2 box2DVec2(aVelocity.x, aVelocity.y);
        b2Body_SetLinearVelocity(*aBodyID, box2DVec2);
    }
    void Physics::SetAngularVelocity(const b2BodyId* aBodyID, float aVelocity)
    {
        b2Body_SetAngularVelocity(*aBodyID, aVelocity);
    }

    Math::Vector2f Physics::GetLinearVelocity(const b2BodyId* aBodyID)
    {
        b2Vec2 velocityB2 = b2Body_GetLinearVelocity(*aBodyID);
        return { velocityB2.x, velocityB2.y };
    }
    float Physics::GetAngularVelocity(const b2BodyId* aBodyID)
    {
        float velocity = b2Body_GetAngularVelocity(*aBodyID);
        return velocity;
    }

    Math::Vector2f Physics::GetBodyPosition(const b2BodyId* aBodyID)
    {
        b2Vec2 position = b2Body_GetPosition(*aBodyID);
        return { position.x, position.y };
    }
    float Physics::GetBodyRotation(const b2BodyId* aBodyID)
    {
        b2Rot rotation = b2Body_GetRotation(*aBodyID);
        return b2Rot_GetAngle(rotation);
    }
}
