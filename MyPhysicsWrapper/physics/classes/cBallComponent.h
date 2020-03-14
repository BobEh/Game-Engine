#pragma once
#include <physics/interfaces/iBallComponent.h>
#include "cPhysicsComponent.h"
#include <string>
#include <phys\cRigidBody.h>
#include <phys/shapes.h>
using namespace phys;

namespace nPhysics
{
	class cBallComponent : public iBallComponent
	{
	public:
		cBallComponent(nPhysics::sBallDef theBallDef);
		~cBallComponent();

		virtual void ApplyForce(const glm::vec3& force);

		virtual size_t NumNodes();
		virtual bool GetNodeRadius(size_t index, float& radiusOut);
		virtual bool GetNodePosition(size_t index, glm::vec3& positionOut);

		virtual void GetTransform(glm::mat4& transformOut);
		virtual void GetPosition(glm::vec3& positionOut);
		virtual void SetPosition(glm::vec3 positionIn);
		virtual void GetVelocity(glm::vec3& velocityOut);
		virtual void GetScale(float& scaleOut);
		virtual int GetMassType();
		virtual void SetMassType(int physicsType);
		virtual std::string GetPlaneType();
		cRigidBody* mBody;
	private:
		//phys::cRigidBody rigidBody;
		//glm::mat4 transform;
		//glm::vec3 position;
		//glm::vec3 velocity;
		std::string planeType;
		int _physicsType;
	};
}