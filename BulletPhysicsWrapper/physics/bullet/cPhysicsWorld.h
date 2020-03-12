#pragma once
#include <physics/interfaces/iPhysicsWorld.h>
#include <physics/interfaces/iCollisionListener.h>
#include "btBulletDynamicsCommon.h"
#include <vector>
#include <cBallComponent.h>
#include <cPlaneComponent.h>

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

		void SetCollisionListener(iCollisionListener* collisionListener);

	private:
		bool AddRigidBodies(cBallComponent* component);
		bool AddRigidBodies(cPlaneComponent* component);
		bool RemoveRigidBodies(cBallComponent* component);
		bool RemoveRigidBodies(cPlaneComponent* component);
		btDefaultCollisionConfiguration* mCollisionConfiguration;
		btCollisionDispatcher* mDispatcher;
		btBroadphaseInterface* mOverlappingPairCache;
		btSequentialImpulseConstraintSolver* mSolver;
		btDiscreteDynamicsWorld* mDynamicsWorld;
		//std::vector<nPhysics::iPhysicsComponent*> componentVec;
		iCollisionListener* collisionListener;
		//phys::cWorld* mWorld;
	};
}