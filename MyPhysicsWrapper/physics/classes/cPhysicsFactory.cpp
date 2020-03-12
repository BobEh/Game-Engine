#include "cPhysicsFactory.h"
#include "cPlaneComponent.h"
#include "cClothComponent.h"

namespace nPhysics
{
	cPhysicsFactory::cPhysicsFactory()
	{

	}
	cPhysicsFactory::~cPhysicsFactory()
	{
	}
	iPhysicsWorld* cPhysicsFactory::CreateWorld()
	{
		cPhysicsWorld* newWorld = new cPhysicsWorld();

		return newWorld;
	}
	iBallComponent* cPhysicsFactory::CreateBall(const sBallDef& def)
	{
		iBallComponent* newBall = new nPhysics::cBallComponent(def);

		return newBall;
	}
	iPlaneComponent* cPhysicsFactory::CreatePlane(const sPlaneDef& def)
	{
		iPlaneComponent* newPlane = new nPhysics::cPlaneComponent(def);

		return newPlane;
	}
	iClothComponent* cPhysicsFactory::CreateCloth(const sClothDef& def)
	{
		//iClothComponent* newCloth = new nPhysics::cClothComponent(def);

		return new cClothComponent(def);
	}
}