#include "cWorld.h"    // My header
#include "nCollide.h"  // collision detection functions from
#include <iostream>
#include <string>
#include "cSoftBody.h"
                       // REAL-TIME COLLISION DETECTION, ERICSON

namespace phys
{
	cWorld::cWorld() : mDeltaTime(0.03f), mGravity(glm::vec3(0.0f,-5.0f,0.0f))
	{
		// TODO: The constructor should initialize all
		//       internal variables to reasonable values
	}

	cWorld::~cWorld()
	{
		// TODO: Is there anything in your world that needs cleaning up?
	}
	
	void cWorld::GetGravity(glm::vec3& gravityOut)
	{
		gravityOut = mGravity;
	}

	void cWorld::SetGravity(const glm::vec3& gravityIn)
	{
		mGravity = gravityIn;
	}

	std::vector<cCollisionBody*> cWorld::GetBodies()
	{
		return mBodies;
	}

	void cWorld::Update(float dt)
	{
		// TODO:
		// 
		// 1) If we have no bodies, there's nothing to do... return.
		// 2) Integrate each body.
		// 3) Perform collision handling on each unique pair of bodies.
		// 4) Clear the acceleration of each rigid body.
		size_t numBodies = mBodies.size();
		if (numBodies == 0)
		{
			return;
		}
		// Step 1: Integrate
		for (size_t c = 0; c < numBodies; c++)
		{
			IntegrateBody(mBodies[c], dt); // TODO: MAKE THIS!
		}
		// Step 2: Handle Collisions!
		std::vector<std::pair<cCollisionBody*, cCollisionBody*>> collisions;
		for (size_t idxA = 0; idxA < numBodies - 1; idxA++)
		{
			for (size_t idxB = idxA + 1; idxB < numBodies; idxB++)
			{
				if (Collide(mBodies[idxA], mBodies[idxB]))
				{
					collisions.push_back(std::make_pair(mBodies[idxA], mBodies[idxB]));
				}
			}
		}
		
		// Step 3: Clear the accelerations!
		for (size_t idx = 0; idx < numBodies; idx++)
		{
			mBodies[idx]->ClearAccelerations();
		}
		// I don't have the collision listener working....
		// Step 4: Tell everybody about the collisions
		//if (mCollisionListener)
		//{
		//	for (size_t idx = 0; idx < collisions.size(); idx++)
		//	{
		//		mCollisionListener->Collide(collisions[idx].first, collisions[idx].second);
		//	}
		//}
	}

	bool cWorld::AddBody(cCollisionBody* collisionBody)
	{
		// TODO:
		// 
		// 1) Null check
		// 2) Check if we currently have this body.
		//    If yes: Return false to indicate nothing was not added.
		//    If no:  Add it, then return true to indicate it was added.
		if (!collisionBody) return false; // Can't add nothing
		std::vector<cCollisionBody*>::iterator itBody = std::find(mBodies.begin(), mBodies.end(), collisionBody);
		if (itBody == mBodies.end())
		{
			// not already there! add it
			mBodies.push_back(collisionBody);
			return true;
		}
		return false; // nothing added
	}

	bool cWorld::RemoveBody(cCollisionBody* collisionBody)
	{
		// TODO:
		// 
		// 1) Null check
		// 2) Check if we currently have this rigid body.
		//    If yes: Remove it, then return true to indicate it was removed.
		//    If no:  Return false to indicate nothing was removed.
		if (!collisionBody) return false; // Can't remove nothing
		std::vector<cCollisionBody*>::iterator itBody = std::find(mBodies.begin(), mBodies.end(), collisionBody);
		if (itBody == mBodies.end()) return false; // wasn't here. wasn't removed;
		mBodies.erase(itBody);
		return true;
	}

	void cWorld::IntegrateBody(cCollisionBody* body, float dt)
	{
		// 1) figure out what kind of body it is
		// 2) Pass it on the the right specific integration method
		if (body->GetBodyType() == eBodyType::rigid)
		{
			IntegrateRigidBody(dynamic_cast<cRigidBody*>(body), dt);
		}
		else if (body->GetBodyType() == eBodyType::soft)
		{
			cSoftBody* softy = dynamic_cast<cSoftBody*>(body);
			softy->Integrate(dt, mGravity);
		}
	}

	void cWorld::IntegrateRigidBody(cRigidBody* body, float dt)
	{
		// TODO:
		// 
		// 1) Static bodies are not to be integrated!
		// 2) Update the body's previous position.
		// 3) Do some integrating!
		// 4) Apply some linear damping
		if (body->IsStatic()) return;
		body->mPreviousPosition = body->mPosition;

		// Gaffer on games integration - Create a class called mIntegrator and add it to cWorld objects
		//std::cout << "In the integrator position X: " << body->mPosition.x << ", Y: " << body->mPosition.y << ", Z: " << body->mPosition.z << std::endl;
		
		mIntegrator.Euler(body->mPosition, body->mVelocity, body->mAcceleration, mGravity, dt);

	}
	bool cWorld::Collide(cCollisionBody* bodyA, cCollisionBody* bodyB)
	{
		// 1) figure out what kind of body it is
		eBodyType typeA = bodyA->GetBodyType();
		eBodyType typeB = bodyB->GetBodyType();
		if (typeA == eBodyType::rigid && typeB == eBodyType::rigid)
		{
			return CollideRigid(dynamic_cast<cRigidBody*>(bodyA), dynamic_cast<cRigidBody*>(bodyB));
		}
		else if (typeA == eBodyType::soft && typeB == eBodyType::rigid)
		{
			return CollideSoft(dynamic_cast<cSoftBody*>(bodyA), dynamic_cast<cRigidBody*>(bodyB));
		}
		else if (typeA == eBodyType::rigid && typeB == eBodyType::soft)
		{
			return CollideSoft(dynamic_cast<cSoftBody*>(bodyA), dynamic_cast<cRigidBody*>(bodyB));
		}
		return false;
	}
	bool cWorld::CollideRigid(cRigidBody* bodyA, cRigidBody* bodyB)
	{
		// TODO:
		// 
		// 1) Based on shape type, determine which specific collision handling
		//    method to use.
		// 2) Cast up the shapes, call the methods, return the result.
		if (bodyA->IsStatic() && bodyB->IsStatic())
		{
			return false;
		}
		eShapeType shapeTypeA = bodyA->GetShapeType();
		eShapeType shapeTypeB = bodyB->GetShapeType();

		if (shapeTypeA == eShapeType::plane)
		{
			if (shapeTypeB == eShapeType::plane)
			{
				return false;
			}
			if (shapeTypeB == eShapeType::sphere)
			{
				// Casting might need to be const???
				return CollideSpherePlane(bodyB, dynamic_cast<cSphere*>(bodyB->GetShape()), bodyA, dynamic_cast<cPlane*>(bodyA->GetShape()));
			}
		}
		if (shapeTypeA == eShapeType::sphere)
		{
			if (shapeTypeB == eShapeType::sphere)
			{
				return CollideSphereSphere(bodyA, dynamic_cast<cSphere*>(bodyA->GetShape()), bodyB, dynamic_cast<cSphere*>(bodyB->GetShape()));
			}
			if (shapeTypeB == eShapeType::plane)
			{
				// Casting might need to be const???
				return CollideSpherePlane(bodyA, dynamic_cast<cSphere*>(bodyA->GetShape()), bodyB, dynamic_cast<cPlane*>(bodyB->GetShape()));
			}
		}
		return false;
	}

	bool cWorld::CollideSoft(cSoftBody* bodyA, cRigidBody* bodyB)
	{
		// TODO:
		// 
		// 1) Based on shape type, determine which specific collision handling
		//    method to use.
		// 2) Cast up the shapes, call the methods, return the result.
		if (bodyB->IsStatic())
		{
			return false;
		}
		eShapeType shapeTypeB = bodyB->GetShapeType();

		if (shapeTypeB == eShapeType::plane)
		{
			//return false;
			bool result = bodyA->CollideSpherePlane(bodyB, dynamic_cast<cPlane*>(bodyB->GetShape()));
			return result;
		}
		else if (shapeTypeB == eShapeType::sphere)
		{
			bool result = bodyA->CollideSphereCloth(bodyB);
			return result;
		}
		return false;
	}

	bool cWorld::CollideSpherePlane(cRigidBody* sphereBody, cSphere* sphereShape, cRigidBody* planeBody, cPlane* planeShape)
	{
		// TODO:
		// 
		// From our textbook, REAL-TIME COLLISION DETECTION, ERICSON
		// Use intersect_moving_sphere_plane(...inputs...outputs...)
		// to determine if:
		// case A: The sphere did not collide during the timestep.
		// case B: The sphere was already colliding at the beginning of the timestep.
		// case C: The sphere collided during the timestep.
		//
		// case A: Return false to indicate no collision happened.
		//
		// case B: Do the timestep again for this sphere after applying an
		//         impulse that should separate it from the plane.
		// 
		// 1) From our textbook, use closest_point_on_plane(..inputs..) to determine the 
		//    penetration-depth of the sphere at the beginning of the timestep.
		//    (This penetration-depth is the distance the sphere must travel during
		//    the timestep in order to escape the plane.)
		// 2) Use the sphere's center and the closest point on the plane to define
		//    the direction of our impulse vector.
		// 3) Use (penetration-depth / DT) to define the magnitude of our impulse vector.
		//    (The impulse vector is now distance/time ...a velocity!)
		// 4) Apply the impulse vector to sphere velocity.
		// 5) Reset the sphere position.
		// 6) Re-do the integration for the timestep.
		// 7) Return true to indicate a collision has happened.
		// 
		// 
		// case C: Rewind to the point of impact, reflect, then replay.
		//
		// 1) Use the outputs from the Ericson function to determine
		//    and set the sphere position to the point of impact.
		// 2) Reflect the sphere's velocity about the plane's normal vector.
		// 3) Apply some energy loss (to the velocity) in the direction
		//    of the plane's normal vector.
		// 4) Re-integrate the sphere with its new velocity over the remaining
		//    portion of the timestep.
		// 5) Return true to indicate a collision has happened.
		
		glm::vec3 c = sphereBody->mPreviousPosition;
		float r = sphereShape->GetRadius();
		glm::vec3 v = sphereBody->mPosition - sphereBody->mPreviousPosition;
		glm::vec3 n = planeShape->GetNormal();
		float d = planeShape->GetConstant();
		float t(0.0f);
		glm::vec3 q;

		int result = nCollide::intersect_moving_sphere_plane(c, r, v, n, d, t, q);
		//std::cout << "I hit the ground!!!! :O" << std::endl;
		if (result == 0)
		{
			// no collision
			return false;
		}
		if (result == -1)
		{
			// already colliding at the beginning of the timestep

			glm::vec3 pointOnPlane = nCollide::closest_point_on_plane(sphereBody->mPreviousPosition, planeShape->GetNormal(), planeShape->GetConstant());
			// figure out an impulse that should have the sphere escape the plane
			float distance = glm::distance(sphereBody->mPreviousPosition, pointOnPlane);
			float targetDistance = r;
			glm::vec3 impulse = n * (targetDistance - distance) / mDeltaTime;
			//reset
			sphereBody->mPosition = sphereBody->mPreviousPosition;
			//apply impulse
			sphereBody->mVelocity += impulse;
			//re-do the timestep
			IntegrateRigidBody(sphereBody, mDeltaTime);

			return true;
		}
		// it collided

		// reflect
		sphereBody->mVelocity = glm::reflect(sphereBody->mVelocity, planeShape->GetNormal());
		// lose some energy
		glm::vec3 nComponent = glm::proj(sphereBody->mVelocity, planeShape->GetNormal());
		// rewind
		sphereBody->mVelocity -= nComponent * 0.6f;

		sphereBody->mPosition = (c + v * t);
		//integrate
		IntegrateRigidBody(sphereBody, mDeltaTime * (1.0f - t));
		//finished
		return true;
	}

	bool cWorld::CollideSphereSphere(cRigidBody* bodyA, cSphere* shapeA, 
		cRigidBody* bodyB, cSphere* shapeB)
	{
		// TODO:
		// 
		// From our textbook, REAL-TIME COLLISION DETECTION, ERICSON
		// Use intersect_moving_sphere_sphere(...inputs...outputs...)
		// to determine if:
		// case A: The spheres don't collide during the timestep.
		// case B: The spheres were already colliding at the beginning of the timestep.
		// case C: The spheres collided during the timestep.
		//
		// case A: Return false to indicate no collision happened.
		//
		// case B: Do the timestep again for these spheres after
		//         applying an impulse that should separate them.
		// 
		// 1) Determine the penetration-depth of the spheres at the beginning of the timestep.
		//    (This penetration-depth is the distance the spheres must travel during
		//    the timestep in order to separate.)
		// 2) Use the sphere's centers to define the direction of our impulse vector.
		// 3) Use (penetration-depth / DT) to define the magnitude of our impulse vector.
		//    (The impulse vector is now distance/time ...a velocity!)
		// 4) Apply a portion of the impulse vector to sphereA's velocity.
		// 5) Apply a portion of the impulse vector to sphereB's velocity.
		//    (Be sure to apply the impulse in opposing directions.)
		// 6) Reset the spheres' positions.
		// 7) Re-do the integration for the timestep.
		// 8) Return true to indicate a collision has happened.
		// 
		// 
		// case C: 
		//
		// 1) Use the outputs from the Ericson function to determine
		//    and set the spheres positions to the point of impact.
		// 2) Use the inelastic collision response equations from
		//    Wikepedia to set they're velocities post-impact.
		// 3) Re-integrate the spheres with their new velocities
		//    over the remaining portion of the timestep.
		// 4) Return true to indicate a collision has happened.
		
		glm::vec3 cA = bodyA->mPreviousPosition;
		glm::vec3 cB = bodyB->mPreviousPosition;
		glm::vec3 vA = bodyA->mPosition - bodyA->mPreviousPosition;
		glm::vec3 vB = bodyB->mPosition - bodyB->mPreviousPosition;
		float rA = shapeA->GetRadius();
		float rB = shapeB->GetRadius();
		float t(0.0f);

		int result = nCollide::intersect_moving_sphere_sphere(cA, rA, vA, cB, rB, vB, t);
		if (result == 0)
		{
			// no collision
			return false;
		}

		// get the masses
		float ma = bodyA->mMass;
		float mb = bodyB->mMass;
		float mt = ma + mb;

		if (result == -1)
		{
			// already colliding

			float initialDistance = glm::distance(bodyA->mPreviousPosition, bodyB->mPreviousPosition);
			float targetDistance = rA + rB;

			glm::vec3 impulseToA = glm::normalize(bodyA->mPreviousPosition - bodyB->mPreviousPosition);
			impulseToA *= (targetDistance - initialDistance);

			// back to ones
			bodyA->mPosition = bodyA->mPreviousPosition;
			bodyB->mPosition = bodyB->mPreviousPosition;
			// apply the impulse
			bodyA->mVelocity += impulseToA * (mb / mt);
			bodyB->mVelocity -= impulseToA * (ma / mt);

			//integrate
			IntegrateRigidBody(bodyA, mDeltaTime);
			IntegrateRigidBody(bodyB, mDeltaTime);

			return true;
		}

		// collided

		// everybody to ones
		// rewind to point of collision
		bodyA->mPosition = bodyA->mPreviousPosition + vA * t;
		bodyB->mPosition = bodyB->mPreviousPosition + vB * t;

		vA = bodyA->mVelocity;
		vB = bodyB->mVelocity;

		float c = 0.2f;
		bodyA->mVelocity = (c * mb * (vB - vA) + ma * vA + mb * vB) / mt;
		bodyB->mVelocity = (c * ma * (vA - vB) + ma * vA + mb * vB) / mt;

		// integrate
		IntegrateRigidBody(bodyA, mDeltaTime * (1.0f - t));
		IntegrateRigidBody(bodyB, mDeltaTime * (1.0f - t));

		return true;
	}
}