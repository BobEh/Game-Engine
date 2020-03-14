#pragma once
//#include <game_math.h>  // single include header for all glm math stuffs
#include "iShape.h"     // iShape base class for all shapes
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include <physics\interfaces\iRigidBody.h>
//#include "../../include/glm/gtx/quaternion.hpp"
//#include <glm/gtx/quaternion.hpp>
//#include <vec3.hpp>
//#include <mat4x4.hpp>
#include "cCollisionBody.h"

namespace phys
{
	// sRigidBodyDef
	// 
	// Contains all non-shape related information
	// necessary to create a cRigidBody instance.
	struct sRigidBodyDef
	{
		// TODO: Create a default constructor that will
		//       initialize everything to reasonable values.
		// Constructor:
		sRigidBodyDef();

		// TODO: Create any handy other contructors, but only if you really wanna.

		// Mass
		// Must be set to a non-negative value.
		// 0.f will result in a static rigid body.
		// A positive value will result in a dynamic rigid body.
		float Mass;
		// Radius
		float Radius;
		// Position
		// The initial position of the rigid body.
		glm::vec3 Position;
		// Rotation
		glm::vec3 Rotation;
		float Angle;
		// Scale
		glm::vec3 Scale;
		// Velocity
		// The initial linear velocity of the rigid body.
		glm::vec3 Velocity;
	};

	// cRigidBody
	//
	// Contains all values required for a rigid body simulation.
	// 
	// Shapes may be shared between rigid bodies.
	// Does not own the iShape* used to create it.
	// Will not delete the iShape* it contains when it is deleted.
	class cRigidBody : public cCollisionBody
	{
		// cWorld will be operating on cRigidBody values quite a bit
		// We will trust it to do everything correctly.
		friend class cWorld;
	public:
		// Constructor
		// All internal values are initialized according to the sRigidBodyDef
		// A valid iShape* is expected.
		cRigidBody(const sRigidBodyDef& def, iShape* shape);

		// Destructor
		// Will not delete the contained iShape.
		virtual ~cRigidBody();

		// ClearAccelerations
		// Sets the internal acceleration vetor to zero
		virtual void ClearAccelerations();

		// GetTransform
		// Retrieve a rendering-friendly form of position + rotation
		void GetTransform(glm::mat4& transformOut);

		// ApplyForce
		// Apply an external force to this rigid body.
		// The force is applied center-mass, affecting the acceleration.
		void ApplyForce(const glm::vec3& force);

		// TODO:
		// ApplyImpulse
		// Apply an impulse to the center-mass of this rigid body.
		// ApplyImpulse(const glm::vec3& impulse);

		// IsStatic
		// A static rigid body was created with a mass of 0.f.
		// A static rigid body does react to other rigid bodies in the world.
		inline bool IsStatic() { return mMass == 0.f; }

		// GetShape
		// Get this rigid body's shape.
		// Each rigid body must be created with a shape.
		inline iShape* GetShape() { return mShape; }

		// GetShapeType
		// Get the type of shape of this rigid body's shape.
		// Equivalent to GetShape()->GetShapeType()
		inline eShapeType GetShapeType() { return mShape->GetShapeType(); }

		virtual void GetPosition(glm::vec3& positionOut);
		virtual void SetPosition(glm::vec3 positionIn);
		void GetPreviousPosition(glm::vec3& positionOut);
		virtual void GetVelocity(glm::vec3& velocityOut);
		void GetRadius(float& radius);
		void GetScale(float& scaleOut);
		void GetMass(float& mass);
		virtual int GetMassType();
		virtual void SetMassType(int physicsType);
		virtual std::string GetPlaneType();

	private:
		// My shape, expected to be valid.
		// cRigidBody will not delete mShape in its destructor.
		iShape* mShape;

		// During a timestep, the previous position is stored for collision related purposes.
		glm::vec3 mPreviousPosition;
		// Current linear position.
		glm::vec3 mPosition;
		// Current Rotation
		glm::vec3 mRotation;
		// Current Angle
		float mAngle;
		// Current Scale
		glm::vec3 mScale;
		// Current linear velocity.
		glm::vec3 mVelocity;
		// Current linear acceleration.
		glm::vec3 mAcceleration;
		// Mass
		// Expected to be non-negative.
		// A value of 0 infers a static rigid body.
		float mMass;
		//Radius
		float mRadius;
		// Inverse mass
		// Stored for convenience.
		// If mMass is 0, mInvMass will also be 0.
		float mInvMass;

		// Constructors not to be used.
		cRigidBody() = delete;
		cRigidBody(const cRigidBody& other) = delete;
		cRigidBody& operator=(const cRigidBody& other) = delete;
	};
}