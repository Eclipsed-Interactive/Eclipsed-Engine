#include "PhysicsEngine.h"

namespace Eclipse
{
    void PhysicsEngine::DrawPhysicsObjects()
    {
        if (myDrawDebugShapes)
            b2World_Draw(myWorld, &myDebugDraw);
    }

    bool& PhysicsEngine::GetDebugDraw()
    {
        return myDrawDebugShapes;
    }

    bool& PhysicsEngine::GetDebugDrawShapes(DebugDrawTypes aType)
    {
        switch (aType)
        {
        case DebugDrawTypes::drawShapes: return myDebugDraw.drawShapes;
        case DebugDrawTypes::drawJoints: return myDebugDraw.drawJoints;
        case DebugDrawTypes::drawJointExtras: return myDebugDraw.drawJointExtras;
        case DebugDrawTypes::drawBounds: return myDebugDraw.drawBounds;
        case DebugDrawTypes::drawMass: return myDebugDraw.drawMass;
        case DebugDrawTypes::drawBodyNames: return myDebugDraw.drawBodyNames;
        case DebugDrawTypes::drawContacts: return myDebugDraw.drawContacts;
        case DebugDrawTypes::drawGraphColors: return myDebugDraw.drawGraphColors;
        case DebugDrawTypes::drawContactNormals: return myDebugDraw.drawContactNormals;
        case DebugDrawTypes::drawContactImpulses: return myDebugDraw.drawContactImpulses;
        case DebugDrawTypes::drawContactFeatures: return myDebugDraw.drawContactFeatures;
        case DebugDrawTypes::drawFrictionImpulses: return myDebugDraw.drawFrictionImpulses;
        case DebugDrawTypes::drawIslands: return myDebugDraw.drawIslands;

        case DebugDrawTypes::drawQueries: return myDrawQueries;
        }

        return myDrawDebugShapes;
    }
}
