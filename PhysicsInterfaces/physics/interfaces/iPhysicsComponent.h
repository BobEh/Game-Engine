#pragma once
//#include <game_math.h>
//#include <mat4x4.hpp>
#include "eComponentType.h"
#include <glm\ext\matrix_float4x4.hpp>
#include <string>
#include <glm/gtx/quaternion.hpp>
//#include <matrix.hpp>

namespace nPhysics
{
	class iPhysicsComponent
	{
	public:
		virtual ~iPhysicsComponent() {}
		inline const eComponentType& GetComponentType() { return mComponentType; }

		//virtual int GetMassType() = 0;
		//virtual void SetMassType(int physicsType) = 0;

		virtual void ApplyForce(const glm::vec3& force) = 0;

		virtual void GetTransform(glm::mat4& transformOut) = 0;

		virtual void GetPosition(glm::vec3& positionOut) = 0;
		virtual void SetPosition(glm::vec3 positionIn) = 0;
		virtual void GetScale(float& scaleOut) = 0;
		virtual void GetVelocity(glm::vec3& velocityOut) = 0;

		virtual size_t NumNodes() = 0;
		virtual bool GetNodeRadius(size_t index, float& radiusOut) = 0;
		virtual bool GetNodePosition(size_t index, glm::vec3& positionOut) = 0;

		//virtual std::string GetPlaneType() = 0;
	
		iPhysicsComponent(eComponentType componentType)
			: mComponentType(componentType) {}
		iPhysicsComponent() {}
	private:
		eComponentType mComponentType;

		// get rid of these!
		//iPhysicsComponent() = delete;
		//iPhysicsComponent(const iPhysicsComponent& other) = delete;
		//iPhysicsComponent& operator=(const iPhysicsComponent& other) = delete;
	};
}