#include "cBallComponent.h"

nPhysics::cBallComponent::~cBallComponent()
{
}

void nPhysics::cBallComponent::GetTransform(glm::mat4& transformOut)
{
	mBody->GetTransform(transformOut);
}

void nPhysics::cBallComponent::GetPosition(glm::vec3& positionOut)
{
	mBody->GetPosition(positionOut);
}

void nPhysics::cBallComponent::SetPosition(glm::vec3 positionIn)
{
	mBody->SetPosition(positionIn);
}

void nPhysics::cBallComponent::ApplyForce(const glm::vec3& force)
{
	mBody->ApplyForce(force);
}

size_t nPhysics::cBallComponent::NumNodes()
{
	return size_t();
}

bool nPhysics::cBallComponent::GetNodeRadius(size_t index, float& radiusOut)
{
	return false;
}

bool nPhysics::cBallComponent::GetNodePosition(size_t index, glm::vec3& positionOut)
{
	return false;
}

void nPhysics::cBallComponent::GetVelocity(glm::vec3& velocityOut)
{
	mBody->GetVelocity(velocityOut);
}

void nPhysics::cBallComponent::GetScale(float& scaleOut)
{
	mBody->GetScale(scaleOut);
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

nPhysics::cBallComponent::cBallComponent(nPhysics::sBallDef theBallDef)
{
	phys::sRigidBodyDef physDef;
	physDef.Position = theBallDef.Position;
	physDef.Mass = theBallDef.Mass;
	physDef.Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	physDef.Rotation = theBallDef.Rotation;
	physDef.Angle = theBallDef.Angle;
	physDef.Scale = theBallDef.Scale;
	physDef.Radius = theBallDef.Radius;
	phys::iShape* shape = new phys::cSphere(theBallDef.Radius);
	mBody = new phys::cRigidBody(physDef, shape);
	
}
