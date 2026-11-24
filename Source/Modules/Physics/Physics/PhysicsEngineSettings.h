#pragma once

#include <vector>
#include "Core/Math/Vector/Vector2.h"
#include "Core/GUID.h"

#include <string>

//#include "Core/Reflection/SerializedEnum.hpp"
#include <unordered_map>
#include <array>

typedef struct BodyId
{
	int32_t index1;
	uint16_t world0;
	uint16_t generation;
} BodyId;

typedef struct ShapeId
{
	int32_t index1;
	uint16_t world0;
	uint16_t generation;
} ShapeId;


#define MAX_LAYERS 16
namespace Eclipse
{
	constexpr int layerCount = 8;
	enum class Layer
	{
		Default = 1 << 0,
		Player = 1 << 1,
		Enemy = 1 << 2,
		Projectile = 1 << 3,
		Ground = 1 << 4,
		Trigger = 1 << 5,
		IgnoreRaycast = 1 << 6,
		UI = 1 << 7,
		All = (1 << layerCount) - 1
	};

	enum DebugDrawTypes
	{
		drawShapes,
		drawJoints,
		drawJointExtras,
		drawBounds,
		drawMass,
		drawBodyNames,
		drawContacts,
		drawGraphColors,
		drawContactNormals,
		drawContactImpulses,
		drawContactFeatures,
		drawFrictionImpulses,
		drawIslands,

		drawQueries,
	};

	enum Box2DBodyType
	{
		StaticBody = 0,
		KinematicBody = 1,
		DynamicBody = 2
	};

	struct RigidBodySettings
	{
		Box2DBodyType BodyType = StaticBody;

		bool LockRotation = false;
		bool LockXPos = false;
		bool LockYPos = false;
	};

	struct PhysMaterial
	{
		float friction = 0.f;
		float restitusion = 0.f;
	};

	struct UserData
	{
		GUID gameobject;
	};

	struct HitResult
	{
		Math::Vector2f point;
		Math::Vector2f normal;
		float fraction;
		GUID gameobject;
	};

	struct HitResults
	{
		std::vector<HitResult> results;
	};

	struct Ray
	{
		Math::Vector2f position;
		Math::Vector2f direction;
	};

	enum class BodyType
	{
		Static = 0,
		Kinematic = 1,
		Dynamic = 2,
		Count,
	};
}
