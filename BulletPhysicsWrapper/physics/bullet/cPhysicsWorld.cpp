#include "cPhysicsWorld.h"

namespace nPhysics
{
	cPhysicsWorld::~cPhysicsWorld()
	{
		delete mCollisionConfiguration;
		delete mDispatcher;
		delete mOverlappingPairCache;
		delete mSolver;
		delete mDynamicsWorld;
		if (collisionListener)
		{
			delete collisionListener;
			collisionListener = 0;
		}
	}
	cPhysicsWorld::cPhysicsWorld()
	{
		collisionListener = 0;
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		mCollisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		mOverlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		mSolver = new btSequentialImpulseConstraintSolver;

		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration);

		mDynamicsWorld->setGravity(btVector3(0, -10, 0));
	}
	void cPhysicsWorld::Update(float deltaTime)
	{
		mDynamicsWorld->stepSimulation(deltaTime, 10);
		if (collisionListener)
		{
			// TODO: COLLISION LISTENING STUFF

		}
	}
	bool cPhysicsWorld::AddComponent(iPhysicsComponent* component)
	{
		if (!component)
		{
			return false; // can't add nothing!
		}
		switch (component->GetComponentType())
		{
		case eComponentType::ball:
			return  AddRigidBodies(dynamic_cast<cBallComponent*>(component));
		case eComponentType::plane:
			return AddRigidBodies(dynamic_cast<cPlaneComponent*>(component));
		default:
			break;
		}
		return false;
	}
	bool cPhysicsWorld::RemoveComponent(iPhysicsComponent* component)
	{
		//// find the component
		//for (int i = 0; i < this->componentVec.size(); i++)
		//{
		//	if (this->componentVec.at(i) == component)
		//	{
		//		this->componentVec.erase(componentVec.begin(), componentVec.begin()+i);
		//		return true;
		//	}
		//}
		//// it wasn't the to begin with
		//return false;
		if (!component)
		{
			return false;
		}
		//for (int i = 0; i < this->componentVec.size(); i++)
		//{
		//	if (this->componentVec.at(i) == component)
		//	{
		//		return false;
		//	}
		//}
		switch (component->GetComponentType())
		{
		case eComponentType::ball:
			return RemoveRigidBodies(dynamic_cast<cBallComponent*>(component));
		case eComponentType::plane:
			return RemoveRigidBodies(dynamic_cast<cPlaneComponent*>(component));
		default:
			break;
		}
		
		return false;
	}
	void cPhysicsWorld::SetCollisionListener(iCollisionListener* _collisionListener)
	{
		if (collisionListener)
		{
			delete collisionListener;
			collisionListener = 0;
		}
		if (_collisionListener)
		{
			
		}
	}
	bool cPhysicsWorld::AddRigidBodies(cBallComponent* component)
	{
		if (!component)
		{
			return false;
		}
		mDynamicsWorld->addRigidBody(component->mBody);
		return true;
	}
	bool cPhysicsWorld::AddRigidBodies(cPlaneComponent* component)
	{
		if (!component)
		{
			return false;
		}
		mDynamicsWorld->addRigidBody(component->mBody);
		return true;
	}
	bool cPhysicsWorld::RemoveRigidBodies(cBallComponent* component)
	{
		if (!component)
		{
			return false;
		}
		mDynamicsWorld->removeRigidBody(component->mBody);
		return true;
	}
	bool cPhysicsWorld::RemoveRigidBodies(cPlaneComponent* component)
	{
		if (!component)
		{
			return false;
		}
		mDynamicsWorld->removeRigidBody(component->mBody);
		return true;
	}
	//void cPhysicsWorld::SetGravity(glm::vec3 gravity)
	//{
	//	mWorld->SetGravity(gravity);
	//}
	//std::vector<nPhysics::iPhysicsComponent*> cPhysicsWorld::GetComponentVec()
	//{
	//	return this->componentVec;
	//}
	//std::vector<phys::cRigidBody*> cPhysicsWorld::GetWorldBodiesVec()
	//{
	//	return mWorld->GetBodies();
	//}
	//phys::cWorld* cPhysicsWorld::GetWorld()
	//{
	//	return mWorld;
	//}
}