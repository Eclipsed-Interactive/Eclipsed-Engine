#include "PointLight2DCommand.h"

#include "EclipsedEngine/Components/Rendering/Lights/PointLight2D.h"

namespace Eclipse
{
    void PointLight2DCommand::Execute()
    {
        myLight->Draw();
    }
}