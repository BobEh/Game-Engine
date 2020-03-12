#include "cBallComponent.h"
#include "nConvert.h"

nPhysics::cBallComponent::cBallComponent(nPhysics::sBallDef theBallDef)
{
	btCollisionShape* shape = new btSphereShape(theBallDef.Radius);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(nConvert::ToBullet(theBallDef.Position));

	btScalar mass(theBallDef.Mass);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
	{
		shape->calculateLocalInertia(mass, localInertia);
	}

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	rbInfo.m_restitution = 1.0f;
	mBody = new btRigidBody(rbInfo);
	mBody->setUserPointer(this);
}

nPhysics::cBallComponent::~cBallComponent()
{
	mBody->setUserPointer(0);
	delete mBody->getMotionState();
	delete mBody;
	mBody = 0;
}

void nPhysics::cBallComponent::GetTransform(glm::mat4& transformOut)
{
	btTransform transform;
	mBody->getMotionState()->getWorldTransform(transform);
	nConvert::ToSimple(transform, transformOut);
}

void nPhysics::cBallComponent::ApplyForce(const glm::vec3& force)
{
	mBody->activate(true);
	mBody->applyCentralForce(nConvert::ToBullet(force));
}

void nPhysics::cBallComponent::GetPosition(glm::vec3& positionOut)
{
	btTransform thePosition;

	mBody->getMotionState()->getWorldTransform(thePosition);

	positionOut = glm::vec3(thePosition.getOrigin().getX(), thePosition.getOrigin().getY(), thePosition.getOrigin().getZ());
}

void nPhysics::cBallComponent::SetPosition(glm::vec3 positionIn)
{
	position = positionIn;
}

void nPhysics::cBallComponent::GetVelocity(glm::vec3& velocityOut)
{
	velocityOut = velocity;
}

int nPhysics::cBallComponent::GetMassType()
{
	return _physicsType;
}

void nPhysics::cBallComponent::SetMassType(int physicsType)
{
	_physicsType = physicsType;
}

std::string nPhysics::cBallComponent::GetPlaneType()
{
	return planeType;
}
