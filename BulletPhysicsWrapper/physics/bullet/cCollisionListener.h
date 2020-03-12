#pragma once
#include <physics\interfaces\iPhysicsComponent.h>
#include <physics\interfaces\iCollisionListener.h>
#include "btBulletDynamicsCommon.h"

namespace nPhysics
{
	class cWrappedCollisionListener
	{
	public:
		cWrappedCollisionListener(iCollisionListener* collisionListener);
		~cWrappedCollisionListener();

		virtual void Collide(btRigidBody* compA, btRigidBody* compB);
	private:
		iCollisionListener* mCollisionListener;
	};
}