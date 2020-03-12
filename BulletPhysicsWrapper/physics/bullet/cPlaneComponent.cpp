#include "cPlaneComponent.h"
#include "nConvert.h"

nPhysics::cPlaneComponent::cPlaneComponent(nPhysics::sPlaneDef thePlaneDef)
{
	btCollisionShape* shape = new btStaticPlaneShape(nConvert::ToBullet(thePlaneDef.Normal), thePlaneDef.Constant);

	btTransform transform;
	transform.setIdentity();

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.f, myMotionState, shape, btVector3(0, 0, 0));
	rbInfo.m_restitution = 0.8f;
	mBody = new btRigidBody(rbInfo);
	mBody->setUserPointer(this);
}

nPhysics::cPlaneComponent::~cPlaneComponent()
{
	mBody->setUserPointer(0);
	delete mBody->getCollisionShape();
	delete mBody->getMotionState();
	delete mBody;
}

void nPhysics::cPlaneComponent::GetTransform(glm::mat4& transformOut)
{
	btTransform transform;
	mBody->getMotionState()->getWorldTransform(transform);
	nConvert::ToSimple(transform, transformOut);
}

void nPhysics::cPlaneComponent::GetPosition(glm::vec3& positionOut)
{
	positionOut = position;
}

void nPhysics::cPlaneComponent::SetPosition(glm::vec3 positionIn)
{
	position = positionIn;
}

void nPhysics::cPlaneComponent::GetVelocity(glm::vec3& velocityOut)
{
	velocityOut = velocity;
}

int nPhysics::cPlaneComponent::GetMassType()
{
	return _physicsType;
}

void nPhysics::cPlaneComponent::SetMassType(int physicsType)
{
	_physicsType = physicsType;
}

std::string nPhysics::cPlaneComponent::GetPlaneType()
{
	return planeType;
}

void nPhysics::cPlaneComponent::ApplyForce(const glm::vec3& force)
{
	//mBody->activate(true);
	//mBody->applyCentralForce(nConvert::ToBullet(force));
}

