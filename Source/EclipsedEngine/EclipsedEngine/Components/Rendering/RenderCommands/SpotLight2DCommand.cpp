#include "SpotLight2DCommand.h"

#include "EclipsedEngine/Components/Rendering/Lights/SpotLight2D.h"

namespace Eclipse
{
    void SpotLight2DCommand::Execute()
    {
        myLight->Draw();
    }
}