#pragma once
#include <physics\interfaces\iPhysicsComponent.h>
#include <phys/iCollisionListener.h>

namespace nPhysics
{
	class cCollisionListener : public phys::iCollisionListenerLibrary
	{
	public:
		~cCollisionListener();
		void Collide(iPhysicsComponent* compA, iPhysicsComponent* compB);
	};
}