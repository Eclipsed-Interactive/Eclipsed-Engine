#include "PhysicsEngine.h"

namespace Eclipse
{
        void PhysicsEngine::CheckCollisions()
    {
        b2ContactEvents contactEvents = b2World_GetContactEvents(myWorld);

        for (int i = 0; i < contactEvents.beginCount; ++i)
        {
            b2ContactBeginTouchEvent event = contactEvents.beginEvents[i];
            HandleBeginContacts(event);
        }
        // for (int i = 0; i < contactEvents.hitCount; ++i)
        // {
        //     b2ContactBeginTouchEvent event = contactEvents.beginEvents[i];
        //     HandleBeginContacts(event);
        // }
        for (int i = 0; i < contactEvents.endCount; ++i)
        {
            b2ContactEndTouchEvent event = contactEvents.endEvents[i];
            HandleEndContacts(event);
        }
    }

    float MyRayCastCallback(b2ShapeId aShape, b2Vec2 aPoint, b2Vec2 aNormal, float aFraction, void* aContext)
    {
        HitResults* myContext = reinterpret_cast<HitResults*>(aContext);

        HitResult& hitContext = myContext->results.emplace_back();

        hitContext.point = { aPoint.x, aPoint.y };
        hitContext.normal = { aNormal.x, aNormal.y };
        hitContext.fraction = aFraction;

        const b2BodyId bodyIdA = b2Shape_GetBody(aShape);
        void* userData = b2Body_GetUserData(bodyIdA);

        if (userData)
            hitContext.gameobject = reinterpret_cast<UserData*>(userData)->gameobject;

        return aFraction;
    }

    bool MyShapeOverlapCallback(b2ShapeId aShape, void* aContext)
    {
        HitResults* myContext = reinterpret_cast<HitResults*>(aContext);

        HitResult& hitContext = myContext->results.emplace_back();

        const b2BodyId bodyIdA = b2Shape_GetBody(aShape);
        void* userData = b2Body_GetUserData(bodyIdA);

        if (userData)
            hitContext.gameobject = reinterpret_cast<UserData*>(userData)->gameobject;

        return true;
    }

    bool PhysicsEngine::RayCast(const Ray& aRay, HitResults& aHitResults, float length, Layer aLayerMask)
    {
        b2QueryFilter filter = b2DefaultQueryFilter();
        filter.maskBits = static_cast<uint64_t>(aLayerMask);
        filter.categoryBits = static_cast<uint64_t>(Layer::All);

        HitResults castContext;
        b2Vec2 translation = b2MulSV(length, b2Normalize(b2Vec2(aRay.direction.x, aRay.direction.y)));
        b2World_CastRay(myWorld, b2Vec2(aRay.position.x, aRay.position.y), translation, filter, MyRayCastCallback, &castContext);

        if (!castContext.results.empty())
        {
            size_t size = sizeof(HitResult) * castContext.results.size();

            aHitResults.results.resize(castContext.results.size());
            std::memcpy(aHitResults.results.data(), castContext.results.data(), size);

            return true;
        }
        return false;
    }

    bool PhysicsEngine::OverlapBox(const Math::Vector2f& aPositon, const Math::Vector2f& aHalfExent, HitResults& aHitResults, Layer aLayerMask)
    {
        b2AABB aabb;

        Math::Vector2f lowerBoundAABB = aPositon - aHalfExent;
        Math::Vector2f upperBoundAABB = aPositon + aHalfExent;

        aabb.lowerBound = b2Vec2(lowerBoundAABB.x, lowerBoundAABB.y);
        aabb.upperBound = b2Vec2(upperBoundAABB.x, upperBoundAABB.y);

        b2QueryFilter filter = b2DefaultQueryFilter();
        filter.maskBits = static_cast<uint64_t>(aLayerMask);
        filter.categoryBits = static_cast<uint64_t>(Layer::All);

        b2World_OverlapAABB(myWorld, aabb, filter, MyShapeOverlapCallback, &aHitResults);
        if (!aHitResults.results.empty())
            return true;

        return false;
    }

    bool PhysicsEngine::OverlapSphere(const Math::Vector2f& aPositon, float aRadius, HitResults& aHitResults, Layer aLayerMask)
    {
        b2Circle circle({ aPositon.x, aPositon.y }, aRadius);
        b2ShapeProxy proxy = b2MakeProxy(&circle.center, 1, circle.radius);

        b2QueryFilter filter = b2DefaultQueryFilter();
        filter.maskBits = static_cast<uint64_t>(aLayerMask);
        filter.categoryBits = static_cast<uint64_t>(Layer::All);

        b2World_OverlapShape(myWorld, &proxy, filter, MyShapeOverlapCallback, &aHitResults);

        if (myDrawDebugShapes && myDrawQueries)
            myDebugDraw.DrawCircleFcn(b2Vec2(aPositon.x, aPositon.y), aRadius, b2HexColor::b2_colorBlack, nullptr);

        if (!aHitResults.results.empty())
            return true;

        return false;
    }
}
