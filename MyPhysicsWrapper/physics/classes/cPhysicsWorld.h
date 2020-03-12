#pragma once
#include <physics/interfaces/iPhysicsWorld.h>
#include <phys/cWorld.h>
#include "cBallComponent.h"
#include "cPlaneComponent.h"
#include "cClothComponent.h"
//#include <physics/interfaces/iCollisionListener.h>

namespace nPhysics
{
	class cPhysicsWorld : public iPhysicsWorld
	{
	public:
		~cPhysicsWorld();
		cPhysicsWorld();

		virtual void Update(float dt);
		virtual bool AddComponent(iPhysicsComponent* component);
		virtual bool RemoveComponent(iPhysicsComponent* component);
		//virtual void SetGravity(glm::vec3 gravity);
		//virtual std::vector<nPhysics::iPhysicsComponent*> GetComponentVec();
		//virtual std::vector<phys::cRigidBody*> GetWorldBodiesVec();
		//virtual phys::cWorld* GetWorld();

	private:
		bool AddRigidBodies(cBallComponent* component);
		bool AddRigidBodies(cPlaneComponent* component);
		bool RemoveRigidBodies(cBallComponent* component);
		bool RemoveRigidBodies(cPlaneComponent* component);
		bool AddSoftBodies(cClothComponent* component);
		bool RemoveSoftBodies(cClothComponent* component);

		std::vector<nPhysics::iPhysicsComponent*> componentVec;
		//iCollisionListener* collisionListener;
		phys::cWorld* mWorld;
	};
}