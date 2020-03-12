#pragma once
#include <physics/interfaces/iPhysicsFactory.h>
#include "cBallComponent.h"
#include "cPhysicsWorld.h"

namespace nPhysics
{
	class cPhysicsFactory : public iPhysicsFactory
	{
	public:
		cPhysicsFactory();
		~cPhysicsFactory();
		iPhysicsWorld* CreateWorld();
		// components
		iBallComponent* CreateBall(const sBallDef& def);
		iPlaneComponent* CreatePlane(const sPlaneDef& def);
		iClothComponent* CreateCloth(const sClothDef& def);
	};
}