#include "cCollisionListener.h"
#include <iostream>

nPhysics::cWrappedCollisionListener::~cWrappedCollisionListener()
{
	mCollisionListener = 0;
}

void nPhysics::cWrappedCollisionListener::Collide(btRigidBody* compA, btRigidBody* compB)
{
	if (!compA || !compB)
	{
		std::cout << "cWrappedCollisionListener received a null body" << std::endl;
	}
	mCollisionListener->Collide(reinterpret_cast<iPhysicsComponent*>(compA->getUserPointer()), reinterpret_cast<iPhysicsComponent*>(compB->getUserPointer()));
}

nPhysics::cWrappedCollisionListener::cWrappedCollisionListener(iCollisionListener* collisionListener)
{
	mCollisionListener = collisionListener;
}
