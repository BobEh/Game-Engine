#pragma once

#include <physics/interfaces/iPhysicsComponent.h>

namespace nPhysics
{
	class cPhysicsComponent : public iPhysicsComponent
	{
	public:
		~cPhysicsComponent();
		inline const eComponentType& GetComponentType() { return mComponentType; }

		void GetTransform(glm::mat4& transformOut);

	protected:
		cPhysicsComponent(eComponentType componentType);
	private:
		eComponentType mComponentType;
	};
}