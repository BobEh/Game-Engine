#pragma once
//#include <game_math.h>
#include "iPhysicsComponent.h"

namespace nPhysics
{
	struct sPlaneDef
	{
		float Constant;
		float Mass;
		glm::vec3 Normal;
		glm::vec3 Position;
		glm::vec3 Rotation;
		float Angle;
		glm::vec3 Scale;
	};
	class iPlaneComponent : public iPhysicsComponent
	{
	public:
		virtual ~iPlaneComponent() {}
		virtual void GetPosition(glm::vec3& positionOut) = 0;

	protected:
		iPlaneComponent() : iPhysicsComponent(eComponentType::plane) {}
	private:
		//iPlaneComponent() = delete;
		iPlaneComponent(const iPlaneComponent& other) = delete;
		iPlaneComponent& operator=(const iPlaneComponent& other) = delete;
	};
}