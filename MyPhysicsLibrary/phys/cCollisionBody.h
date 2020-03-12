#pragma once
#include "eBodyType.h"

namespace phys
{
	class cCollisionBody
	{
	public:
		// Destructor
		virtual ~cCollisionBody() {}

		//GetBodyType
		// Returns the eBodyType indicating the specific shape.
		inline const eBodyType& GetBodyType() { return mBodyType; }

		// ClearAccelerations
		// Used by cWorld post integration
		virtual void ClearAccelerations() = 0;

	protected:

		// Constructor
		// Expacted to be called only by the constructors of subclasses.
		// The calling subclass is expected to pass the correct eBodyType.
		cCollisionBody(eBodyType bodyType) : mBodyType(bodyType)
		{

		}
	private:
		// The body type identifier.
		eBodyType mBodyType;

		// Constructors not to be used.
		cCollisionBody() = delete;
		cCollisionBody(const cCollisionBody& other) = delete;
		cCollisionBody& operator=(const cCollisionBody& other) = delete;
	};
}