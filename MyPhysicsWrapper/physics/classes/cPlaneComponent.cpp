#include "cPlaneComponent.h"

nPhysics::cPlaneComponent::~cPlaneComponent()
{
}

void nPhysics::cPlaneComponent::GetTransform(glm::mat4& transformOut)
{
	mBody->GetTransform(transformOut);
}

void nPhysics::cPlaneComponent::GetPosition(glm::vec3& positionOut)
{
	mBody->GetPosition(positionOut);
}

void nPhysics::cPlaneComponent::SetPosition(glm::vec3 positionIn)
{
	position = positionIn;
}

void nPhysics::cPlaneComponent::ApplyForce(const glm::vec3& force)
{
	//mBody->ApplyForce(force);
}

void nPhysics::cPlaneComponent::GetVelocity(glm::vec3& velocityOut)
{
	velocityOut = velocity;
}

void nPhysics::cPlaneComponent::GetScale(float& scaleOut)
{
	mBody->GetScale(scaleOut);
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

nPhysics::cPlaneComponent::cPlaneComponent(nPhysics::sPlaneDef thePlaneDef)
{
	phys::sRigidBodyDef physDef;
	physDef.Position = thePlaneDef.Position;
	physDef.Mass = thePlaneDef.Mass;
	physDef.Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	physDef.Angle = thePlaneDef.Angle;
	physDef.Rotation = thePlaneDef.Rotation;
	physDef.Scale = thePlaneDef.Scale;
	phys::iShape* shape = new phys::cPlane(thePlaneDef.Normal, thePlaneDef.Constant);
	mBody = new phys::cRigidBody(physDef, shape);
}

size_t nPhysics::cPlaneComponent::NumNodes()
{
	return size_t();
}

bool nPhysics::cPlaneComponent::GetNodeRadius(size_t index, float& radiusOut)
{
	return false;
}

bool nPhysics::cPlaneComponent::GetNodePosition(size_t index, glm::vec3& positionOut)
{
	return false;
}
