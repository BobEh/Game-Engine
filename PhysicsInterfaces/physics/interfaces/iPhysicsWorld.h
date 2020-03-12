#pragma once
//#include <game_math.h>
#include "iPhysicsComponent.h"
#include "iRigidBody.h"
#include <vector>
#include "../../../MyPhysicsLibrary/phys/cRigidBody.h"
#include "../../../MyPhysicsLibrary/phys/cWorld.h"

namespace nPhysics
{
	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}
		
		virtual void Update(float dt) = 0;
		virtual bool AddComponent(iPhysicsComponent* component) = 0;
		virtual bool RemoveComponent(iPhysicsComponent* component) = 0;
		//virtual void SetGravity(glm::vec3 gravity) = 0;
		//virtual std::vector<iPhysicsComponent*> GetComponentVec() = 0;
		//virtual std::vector<phys::cRigidBody*> GetWorldBodiesVec() = 0;
		//virtual phys::cWorld* GetWorld() = 0;
	};
}