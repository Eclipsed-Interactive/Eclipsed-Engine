#include "ComponentForcelink.h"

#include "EclipsedEngine/Reflection/Registry/ComponentRegistry.h"

#include "EclipsedEngine/Components/Transform2D.h"
#include "EclipsedEngine/Components/Physics/Collider2D.h"
#include "EclipsedEngine/Components/Physics/BoxCollider2D.h"
#include "EclipsedEngine/Components/Physics/CapsuleCollider2D.h"
#include "EclipsedEngine/Components/Physics/CircleCollider2D.h"
#include "EclipsedEngine/Components/Physics/PolygonCollider2D.h"
#include "EclipsedEngine/Components/Physics/RigidBody2D.h"

#include "EclipsedEngine/Components/PlayerSpawner.h"

#include "EclipsedEngine/Components/Rendering/Camera.h"
#include "EclipsedEngine/Components/Rendering/SpriteRenderer2D.h"
#include "EclipsedEngine/Components/Rendering/SpriteSheetAnimator2D.h"
#include "EclipsedEngine/Components/Rendering/TextRenderer.h"

#include "EclipsedEngine/Components/UI/Button.h"
#include "EclipsedEngine/Components/UI/Canvas.h"
#include "EclipsedEngine/Components/UI/RectTransform.h"
#include "EclipsedEngine/Components/UI/UIImage.h"

#include "EclipsedEngine/Components/Audio/AudioBank.h"
#include "EclipsedEngine/Components/Audio/AudioEmitter.h"
#include "EclipsedEngine/Components/Audio/AudioEvent.h"
#include "EclipsedEngine/Components/Audio/AudioListener.h"


#ifndef ECLIPSED_EDITOR
#include "Forcelink.h"
#endif

void Eclipse::ComponentForcelink::LinkComponents()
{
	
	COMP_REG(Transform2D);
	COMP_REG(Collider2D);
	COMP_REG(BoxCollider2D);
	COMP_REG(CapsuleCollider2D);
	COMP_REG(CircleCollider2D);
	COMP_REG(PolygonCollider2D);
	COMP_REG(RigidBody2D);
	
	COMP_REG(Camera);
	COMP_REG(SpriteRenderer2D);
	COMP_REG(TextRenderer);
	
	COMP_REG(Button);
	COMP_REG(Canvas);
	COMP_REG(RectTransform);
	COMP_REG(UIImage);

	COMP_REG(AudioEmitter);
	COMP_REG(AudioBank);
	COMP_REG(AudioEvent);
	COMP_REG(AudioListener);
	
	COMP_REG(PlayerSpawner);

#ifndef ECLIPSED_EDITOR
	RegisterComponents();
#endif
}
