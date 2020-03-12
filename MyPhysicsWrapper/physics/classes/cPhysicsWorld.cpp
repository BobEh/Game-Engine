#include "cPhysicsWorld.h"

namespace nPhysics
{
	cPhysicsWorld::~cPhysicsWorld()
	{
	}
	cPhysicsWorld::cPhysicsWorld()
	{
		mWorld = new phys::cWorld();
	}
	void cPhysicsWorld::Update(float deltaTime)
	{
		mWorld->Update(deltaTime);
	}
	bool cPhysicsWorld::AddComponent(iPhysicsComponent* component)
	{
		// make sure it doesn't already exist
		for (int i = 0; i < this->componentVec.size(); i++)
		{
			if (this->componentVec.at(i) == component)
			{
				return false;
			}
		}
		// does not already exist so push it back
		this->componentVec.push_back(component);
		//phys::sRigidBodyDef* componentDef = new phys::sRigidBodyDef();
		//componentDef->Mass = 0.0f;
		//componentDef->Position = glm::vec3(0.0f, 0.0f, 0.0f);
		//componentDef->Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		//phys::iShape* componentShape = NULL;
		switch (component->GetComponentType())
		{
		case eComponentType::ball:
			return  AddRigidBodies(dynamic_cast<cBallComponent*>(component));
		case eComponentType::plane:
			return AddRigidBodies(dynamic_cast<cPlaneComponent*>(component));
		case eComponentType::cloth:
			return AddSoftBodies(dynamic_cast<cClothComponent*>(component));
		default:
			break;
		}
		return false;
		//component->GetPosition(componentDef->Position);
		//
		//phys::cRigidBody* physComponent = new phys::cRigidBody(*componentDef, componentShape);
		//this->mWorld->AddRigidBody(physComponent);
		//return true;
	}
	bool cPhysicsWorld::RemoveComponent(iPhysicsComponent* component)
	{
		// find the component
		for (int i = 0; i < this->componentVec.size(); i++)
		{
			if (this->componentVec.at(i) == component)
			{
				this->componentVec.erase(componentVec.begin(), componentVec.begin()+i);
				componentVec.erase(componentVec.begin() + i);
				return true;
			}
		}
		// it wasn't the to begin with
		switch (component->GetComponentType())
		{
		case eComponentType::ball:
			return RemoveRigidBodies(dynamic_cast<cBallComponent*>(component));
		case eComponentType::plane:
			return RemoveRigidBodies(dynamic_cast<cPlaneComponent*>(component));
		case eComponentType::cloth:
			return RemoveSoftBodies(dynamic_cast<cClothComponent*>(component));
		default:
			break;
		}
		return false;
	}
	bool cPhysicsWorld::AddRigidBodies(cBallComponent* component)
	{
		if (!component)
		{
			return false;
		}
		mWorld->AddBody(component->mBody);
		return true;
	}
	bool cPhysicsWorld::AddRigidBodies(cPlaneComponent* component)
	{
		if (!component)
		{
			return false;
		}
		mWorld->AddBody(component->mBody);
		return true;
	}
	bool cPhysicsWorld::RemoveRigidBodies(cBallComponent* component)
	{
		if (!component)
		{
			return false;
		}
		mWorld->RemoveBody(component->mBody);
		return true;
	}
	bool cPhysicsWorld::RemoveRigidBodies(cPlaneComponent* component)
	{
		if (!component)
		{
			return false;
		}
		mWorld->RemoveBody(component->mBody);
		return true;
	}
	bool cPhysicsWorld::AddSoftBodies(cClothComponent* component)
	{
		if (!component)
		{
			return false;
		}
		return mWorld->AddBody(component->mBody);
	}
	bool cPhysicsWorld::RemoveSoftBodies(cClothComponent* component)
	{
		if (!component)
		{
			return false;
		}
		return mWorld->RemoveBody(component->mBody);
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