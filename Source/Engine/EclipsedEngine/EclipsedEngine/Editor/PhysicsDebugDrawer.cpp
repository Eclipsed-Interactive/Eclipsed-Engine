#include "PhysicsDebugDrawer.h"

#include "GraphicsEngine/OpenGL/DebugDrawers/DebugDrawer.h"

#include "CoreEngine/MainSingleton.h"
#include "CoreEngine/Settings/EngineSettings.h"

#include "CoreEngine/Settings/GraphicsSettings.h"

using namespace Eclipse;

void PhysicsDebugDrawer::DrawPolygon(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context)
{
    for (int i = 0; i < vertexCount - 1; i++)
    {
        int realIndexPOne = i == vertexCount - 1 ? 0 : i + 1;

        Math::Vector2f mathVertex0({ vertices[i].x, vertices[i].y });
        Math::Vector2f mathVertex1({ vertices[realIndexPOne].x, vertices[realIndexPOne].y });

        Math::Color mathColor(color);

        DebugDrawer::DrawLine(mathVertex0, mathVertex1, true, mathColor);
    }
}
void PhysicsDebugDrawer::DrawSolidPolygon(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context)
{
    const Math::Vector2i& resolution = Settings::GraphicsSettings::GetResolution();
    float resRatio = resolution.y / resolution.x;

    Math::Vector2f position({ transform.p.x, transform.p.y });

    for (int i = 0; i < vertexCount; i++)
    {
        int realIndexPOne = i == vertexCount - 1 ? 0 : i + 1;

        Math::Vector2f mathVertex0(Math::Vector2f{ vertices[i].x, vertices[i].y });
        float vertX0 = mathVertex0.x * transform.q.c - mathVertex0.y * transform.q.s;
        float vertY0 = mathVertex0.x * transform.q.s + mathVertex0.y * transform.q.c;

        mathVertex0.x = vertX0;
        mathVertex0.y = vertY0;

        mathVertex0 += position;

        mathVertex0.x += 1;
        mathVertex0.x *= 0.5f;
        mathVertex0.y += 1;
        mathVertex0.y *= 0.5f;


        Math::Vector2f mathVertex1(Math::Vector2f{ vertices[realIndexPOne].x, vertices[realIndexPOne].y });
        float vertX1 = mathVertex1.x * transform.q.c - mathVertex1.y * transform.q.s;
        float vertY1 = mathVertex1.x * transform.q.s + mathVertex1.y * transform.q.c;

        mathVertex1.x = vertX1;
        mathVertex1.y = vertY1;

        mathVertex1 += position;

        mathVertex1.x += 1;
        mathVertex1.x *= 0.5f;
        mathVertex1.y += 1;
        mathVertex1.y *= 0.5f;

        Math::Color mathColor(color);
        mathColor.a = 1;

        DebugDrawer::DrawLine(mathVertex0, mathVertex1, i ? true : false);
    }
}
void PhysicsDebugDrawer::DrawCircle(b2Vec2 center, float radius, b2HexColor color, void* context)
{
    DebugDrawer::DrawCircle(Math::Vector2f((center.x + 1.f) * 0.5f, (center.y + 1.f) * 0.5f), radius, 32);
}
void PhysicsDebugDrawer::DrawSolidCircle(b2Transform transform, float radius, b2HexColor color, void* context)
{
    Math::Vector2f position({ transform.p.x, transform.p.y });

    DebugDrawer::DrawCircle(Math::Vector2f((position.x + 1.f) * 0.5f, (position.y + 1.f) * 0.5f), radius, 32);
}
void PhysicsDebugDrawer::DrawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void* context)
{
    Math::Vector2f direction(p1.x - p2.x, p1.y - p2.y);
    direction.Normalize();

    Math::Vector2f negDir = direction * -1;

    DebugDrawer::DrawHalfCircle(Math::Vector2f((p1.x + 1.f) * 0.5f, (p1.y + 1.f) * 0.5f) - direction * 0.001f, radius, direction, 32);
    DebugDrawer::DrawHalfCircle(Math::Vector2f((p2.x + 1.f) * 0.5f, (p2.y + 1.f) * 0.5f) + direction * 0.001f, radius, negDir, 32);

    Math::Vector2f eclpisePosition1(p1.x, p1.y);
    Math::Vector2f eclpisePosition2(p2.x, p2.y);

    eclpisePosition1.x += 1;
    eclpisePosition1.x *= 0.5f;
    eclpisePosition1.y += 1;
    eclpisePosition1.y *= 0.5f;

    eclpisePosition2.x += 1;
    eclpisePosition2.x *= 0.5f;
    eclpisePosition2.y += 1;
    eclpisePosition2.y *= 0.5f;

    Math::Vector2f rightVec(direction.y, -direction.x);
    Math::Vector2f totalRightDirection(rightVec * radius * 0.5f);

    DebugDrawer::DrawLine(eclpisePosition1 + totalRightDirection, eclpisePosition2 + totalRightDirection);
    DebugDrawer::DrawLine(eclpisePosition1 - totalRightDirection, eclpisePosition2 - totalRightDirection);
}
void PhysicsDebugDrawer::DrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context)
{

}
void PhysicsDebugDrawer::DrawTransform(b2Transform transform, void* context)
{

}

void PhysicsDebugDrawer::Init(b2DebugDraw* aDebugDraw)
{
    b2DebugDraw& derefDebugDraw = *aDebugDraw;

    // TODO: Hardcoded for now should be the cameras bounds in the future
    b2AABB aabb;
    {
        aabb.lowerBound = { -1000, -1000 };
        aabb.upperBound = { 1000, 1000 };
    }

    derefDebugDraw.drawingBounds = aabb;

    derefDebugDraw.DrawPolygonFcn = &PhysicsDebugDrawer::DrawPolygon;
    derefDebugDraw.DrawSolidPolygonFcn = &PhysicsDebugDrawer::DrawSolidPolygon;

    derefDebugDraw.DrawSolidCapsuleFcn = &PhysicsDebugDrawer::DrawSolidCapsule;

    derefDebugDraw.DrawCircleFcn = &PhysicsDebugDrawer::DrawCircle;
    derefDebugDraw.DrawSolidCircleFcn = &PhysicsDebugDrawer::DrawSolidCircle;

    derefDebugDraw.DrawSegmentFcn = &PhysicsDebugDrawer::DrawSegment;
    derefDebugDraw.DrawTransformFcn = &PhysicsDebugDrawer::DrawTransform;

    derefDebugDraw.context = nullptr;
}