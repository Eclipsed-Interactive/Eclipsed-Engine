#pragma once

#include "box2d/box2d.h"

class PhysicsDebugDrawer
{
public:
    static void DrawPolygon(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context);
    static void DrawSolidPolygon(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context);
    static void DrawCircle(b2Vec2 center, float radius, b2HexColor color, void* context);
    static void DrawSolidCircle(b2Transform transform, float radius, b2HexColor color, void* context);

    static void DrawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void* context);

    static void DrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context);
    static void DrawTransform(b2Transform transform, void* context);

    static void Init(b2DebugDraw* aDebugDraw);
};