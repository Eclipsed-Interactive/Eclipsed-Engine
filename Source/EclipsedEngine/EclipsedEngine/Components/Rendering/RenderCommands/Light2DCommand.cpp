#include "Light2DCommand.h"

#include "EclipsedEngine/Components/Rendering/Lights/PointLight2D.h"

namespace Eclipse
{
    void Light2DCommand::Execute()
    {
        myLight->Draw();
    }
}