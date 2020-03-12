#pragma once
//#include <game_math.h>
//#include <vec3.hpp>
#include "iPhysicsComponent.h"

namespace nPhysics
{
	struct sBallDef
	{
		float Mass;
		float Radius;
		glm::vec3 Position;
		glm::vec3 Rotation;
		float Angle;
		glm::vec3 Scale;
	};

	class iBallComponent : public iPhysicsComponent
	{
	public:
		virtual ~iBallComponent() {}
		virtual void GetPosition(glm::vec3& positionOut) = 0;
		
	protected:
		iBallComponent() : iPhysicsComponent(eComponentType::ball) {}
	private:
		//iBallComponent() = delete;
		iBallComponent(const iBallComponent& other) = delete;
		iBallComponent& operator=(const iBallComponent& other) = delete;
	};
}