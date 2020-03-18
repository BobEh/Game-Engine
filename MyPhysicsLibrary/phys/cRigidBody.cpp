#include "cRigidBody.h"  // My header
#include <glm/gtc/matrix_transform.hpp>

namespace phys
{
	sRigidBodyDef::sRigidBodyDef() : Mass(0.0f), Position(glm::vec3(0.0f, 0.0f, 0.0f)), Velocity(glm::vec3(0.0f, 0.0f, 0.0f)), Radius(1.0f)
	{

	} 
	cRigidBody::cRigidBody(const sRigidBodyDef& def, iShape* shape) : cCollisionBody(eBodyType::rigid), mShape(shape), mPosition(def.Position), mVelocity(def.Velocity), mMass(def.Mass), mRotation(def.Rotation), mAngle(def.Angle), mScale(def.Scale), mRadius(def.Radius)
	{
		if (mMass == 0.0f)
		{
			mInvMass = 0.0f;
		}
		else
		{
			mInvMass = 1.f / mMass;
		}
	}

	int cRigidBody::GetMassType()
	{
		return 1;
	}
	void cRigidBody::SetMassType(int physicsType)
	{

	}

	cRigidBody::~cRigidBody()
	{
		delete this;
	}

	void cRigidBody::ClearAccelerations()
	{
		mAcceleration.x = 0.0f;
		mAcceleration.y = 0.0f;
		mAcceleration.z = 0.0f;
	}

	void cRigidBody::GetTransform(glm::mat4& transformOut)
	{
		transformOut = glm::mat4(1.0f);
		// rotations
		// translate is in gtx
		transformOut = glm::translate(transformOut, mPosition);
		//transformOut *= glm::rotate(transformOut, glm::radians(mAngle), mRotation);
		//transformOut *= glm::scale(transformOut, mScale);
		//glm::mat4 matWorld = glm::mat4(1.0f);


		//// ******* TRANSLATION TRANSFORM *********
		//glm::vec3 tempPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		////pCurrentObject->GetComponent()->GetPosition(tempPosition);
		//glm::mat4 componentTrans;
		////pCurrentObject->GetComponent()->GetTransform(componentTrans);
		//componentTrans = glm::translate(glm::mat4(1.0f), mPosition);
		////std::cout << "In Calculate function X: " << pCurrentObject->getPositionXYZ().x << ", Y: " << pCurrentObject->getPositionXYZ().y << ", Z: " << pCurrentObject->getPositionXYZ().z << std::endl;
		//matWorld = matWorld * componentTrans;
		//// ******* TRANSLATION TRANSFORM *********



		//// ******* ROTATION TRANSFORM *********

		//glm::mat4 rotation = glm::mat4(mRotation);
		//matWorld *= rotation;
		//// ******* ROTATION TRANSFORM *********



		//// ******* SCALE TRANSFORM *********
		//glm::mat4 scale = glm::scale(glm::mat4(1.0f),
		//	glm::vec3(pCurrentObject->getScale(),
		//		pCurrentObject->getScale(),
		//		pCurrentObject->getScale()));
		//matWorld = matWorld * scale;
	}

	void cRigidBody::ApplyForce(const glm::vec3& force)
	{
		// TODO: Apply the force to this rigid body's acceleration
		//       Remember... F=ma  ...so an applied acceleration will be f/m
		mAcceleration += force / mInvMass;
	}
	void cRigidBody::GetPosition(glm::vec3& positionOut)
	{
		positionOut = this->mPosition;
	}
	void cRigidBody::SetPosition(glm::vec3 positionIn)
	{
		this->mPosition = positionIn;
	}
	void cRigidBody::GetPreviousPosition(glm::vec3& positionOut)
	{
		positionOut = mPreviousPosition;
	}
	void cRigidBody::GetVelocity(glm::vec3& velocityOut)
	{
		velocityOut = this->mVelocity;
	}
	void cRigidBody::GetRadius(float& radius)
	{
		radius = mRadius;
	}
	void cRigidBody::GetScale(float& scaleOut)
	{
		scaleOut = mScale.x;
	}
	void cRigidBody::GetMass(float& mass)
	{
		mass = mMass;
	}
	std::string cRigidBody::GetPlaneType()
	{
		return std::string();
	}
}
