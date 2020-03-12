#pragma once
#include <physics/interfaces/iPlaneComponent2.h>
#include "cPhysicsComponent.h"
#include <string>
#include "btBulletDynamicsCommon.h"

namespace nPhysics
{
	class cPlaneComponent : public iPlaneComponent
	{
	public:
		cPlaneComponent(nPhysics::sPlaneDef thePlaneDef);
		~cPlaneComponent();

		virtual void GetTransform(glm::mat4& transformOut);
		virtual void ApplyForce(const glm::vec3& force);
		virtual void GetPosition(glm::vec3& positionOut);
		virtual void SetPosition(glm::vec3 positionIn);
		virtual void GetVelocity(glm::vec3& velocityOut);
		virtual int GetMassType();
		virtual void SetMassType(int physicsType);
		virtual std::string GetPlaneType();
		btRigidBody* mBody;
	private:

		//glm::mat4 transform;
		glm::vec3 position;
		glm::vec3 velocity;
		std::string planeType;
		int _physicsType;
	};
}