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
	//mBody->applyCentralForce(nConvert::ToBullet(force));
	//mBody->applyForce(nConvert::ToBullet(force), mBody->getCenterOfMassPosition());
	//mBody->applyTorque(nConvert::ToBullet(force));

	// The "forward" vector is +ve Z
	// (the 4th number is because we need a vec4 later)
	glm::vec4 forwardDirObject = glm::vec4(force, 1.0f);

	glm::mat4 matModel = glm::mat4(1.0f);	// Identity

	// Roation
	// Constructor for the GLM mat4x4 can take a quaternion
	float x, y, z;
	mBody->getWorldTransform().getRotation().getEulerZYX(z, y, x);
	glm::mat4 matRotation = glm::mat4(glm::quat(glm::vec3(x, y, z)));
	matModel *= matRotation;
	// *******************

	// Like in the vertex shader, I mulitly the test points
	// by the model matrix (MUST be a VEC4 because it's a 4x4 matrix)
	glm::vec4 forwardInWorldSpace = matModel * forwardDirObject;

	mBody->applyCentralForce(nConvert::ToBullet(nConvert::ToSimple(mBody->getCenterOfMassPosition()) + glm::vec3(forwardInWorldSpace)));
}

void nPhysics::cBallComponent::GetPosition(glm::vec3& positionOut)
{
	btTransform thePosition;

	mBody->getMotionState()->getWorldTransform(thePosition);

	positionOut = glm::vec3(thePosition.getOrigin().getX(), thePosition.getOrigin().getY(), thePosition.getOrigin().getZ());
}

void nPhysics::cBallComponent::SetPosition(glm::vec3 positionIn)
{
	btTransform initialTransform;

	initialTransform.setOrigin(nConvert::ToBullet(positionIn));
	initialTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f));

	mBody->setWorldTransform(initialTransform);
	mBody->getMotionState()->setWorldTransform(initialTransform);
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
