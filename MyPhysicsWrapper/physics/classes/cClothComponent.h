#pragma once
#include <physics/interfaces/iClothComponent.h>
#include "cPhysicsComponent.h"
#include <string>
#include <phys/cSoftBody.h>
#include <phys/shapes.h>
using namespace phys;

namespace nPhysics
{
	class cClothComponent : public iClothComponent
	{
	public:
		cClothComponent(const sClothDef& theClothDef);
		~cClothComponent();

		virtual void ApplyForce(const glm::vec3& force);

		virtual void GetTransform(glm::mat4& transformOut);
		virtual size_t NumNodes();
		virtual bool GetNodeRadius(size_t index, float& radiusOut);
		virtual bool GetNodePosition(size_t index, glm::vec3& positionOut);
		virtual void GetPosition(glm::vec3& positionOut);
		virtual void SetPosition(glm::vec3 positionIn);
		virtual void GetVelocity(glm::vec3& velocityOut);
		virtual void GetScale(float& scaleOut);
		virtual int GetMassType();
		virtual void SetMassType(int physicsType);
		virtual std::string GetPlaneType();
		cSoftBody* mBody;
	private:

		glm::mat4 transform;
		glm::vec3 position;
		glm::vec3 velocity;
		std::string planeType;
		int _physicsType;
	};
}