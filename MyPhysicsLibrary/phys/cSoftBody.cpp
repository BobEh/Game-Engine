#include "cSoftBody.h"
#include <numeric>
#include "nCollide.h"
#include <glm\gtx\projection.hpp>

namespace phys
{
	cSoftBody::cSpring::cSpring(cNode* nodeA, cNode* nodeB, float springConstant) : 
		NodeA(nodeA), 
		NodeB(nodeB), 
		RestingLength(glm::distance(nodeA->Position, nodeB->Position)), 
		SpringConstant(springConstant), 
		SpringForceAtoB(0.0f,0.0f,0.0f)
	{

	}
	void cSoftBody::cSpring::UpdateSpringForce()
	{
		// HOOKS LAW!
		glm::vec3 sep = NodeB->Position - NodeA->Position;
		float dist = glm::length(sep);
		float x = dist - RestingLength;
		SpringForceAtoB = -SpringConstant * glm::normalize(sep) * x;
	}
	void cSoftBody::cSpring::ApplyForceToNodes()
	{
		if (!NodeB->IsFixed())
		{
			NodeB->Acceleration += SpringForceAtoB / NodeB->Mass;
		}
		if (!NodeA->IsFixed())
		{
			NodeA->Acceleration -= SpringForceAtoB / NodeA->Mass;
		}
		
	}
	cSoftBody::cNode::cNode(const sSoftBodyNodeDef& nodeDef) : Position(nodeDef.Position), PreviousPosition(glm::vec3(0.0f,0.0f,0.0f)), Mass(nodeDef.Mass), Velocity(0.0f,0.0f,0.0f), Acceleration(0.0f,0.0f,0.0f), Radius(nodeDef.Radius)
	{

	}
	bool cSoftBody::cNode::IsNeighbor(cNode* other)
	{
		size_t numSprings = Springs.size();
		for (size_t idx = 0; idx < numSprings; idx++)
		{
			if (Springs[idx]->GetOther(this) == other)
			{
				return true;
			}
		}
		return false;
	}
	cSoftBody::cNode* cSoftBody::cSpring::GetOther(cNode* node)
	{
		return node == NodeA ? NodeB : NodeA;
	}
	void cSoftBody::cNode::CalculateRadius()
	{

		float smallestDistance = std::numeric_limits<float>::max();
		size_t numNeighbors = this->Springs.size();
		for (size_t idx = 0; idx < numNeighbors; idx++)
		{
			// is this the smallest distance?
			float dist = glm::distance(Springs[idx]->GetOther(this)->Position, Position);
			if (dist < smallestDistance)
			{
				smallestDistance = dist;
			}
		}
		Radius = smallestDistance * 0.5f; // TODO: tweak this value, maybe make it a constant somewhere?
	}
	cSoftBody::cSoftBody(const sSoftBodyDef& def) : cCollisionBody(eBodyType::soft)
	{
		size_t numNodes = def.Nodes.size();
		mNodes.resize(numNodes);
		for (size_t idx = 0; idx < numNodes; idx++)
		{
			mNodes[idx] = new cNode(def.Nodes[idx]);
		}
		size_t numSprings = def.Springs.size();
		mSprings.resize(numSprings);
 		for (size_t idx = 0; idx < numSprings; idx++)
		{
			cSpring* spring = new cSpring(
				mNodes[def.Springs[idx].first], 
				mNodes[def.Springs[idx].second], 
				def.SpringConstant);
			mSprings[idx] = spring;
			mNodes[def.Springs[idx].first]->Springs.push_back(spring);
			mNodes[def.Springs[idx].second]->Springs.push_back(spring);
		}
	}
	cSoftBody::~cSoftBody()
	{
		size_t numNodes = mNodes.size();
		for (size_t idx = 0; idx < numNodes; idx++)
		{
			delete mNodes[idx];
		}
		mNodes.clear();
		size_t numSprings = mSprings.size();
		for (size_t idx = 0; idx < numSprings; idx++)
		{
			delete mSprings[idx];
		}
	}
	void cSoftBody::ClearAccelerations()
	{
		size_t numNodes = mNodes.size();
		for (size_t idx = 0; idx < numNodes; idx++)
		{
			mNodes[idx]->Acceleration.x = 0.0f;
			mNodes[idx]->Acceleration.y = 0.0f;
			mNodes[idx]->Acceleration.z = 0.0f;
		}
	}
	//random function
	template <class T>
	T randInRange(T min, T max)
	{
		double value =
			min + static_cast <double> (rand())
			/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
		return static_cast<T>(value);
	};
	glm::vec3 wind = glm::vec3(0.0f);
	void cSoftBody::Integrate(float dt, const glm::vec3& gravity)
	{
		size_t numNodes = mNodes.size();
		// step 1 - gravity
		for (size_t idx = 0; idx < numNodes; idx++)
		{			
			mNodes[idx]->Acceleration = gravity;
		}
		// step 1.5 - wind
		glm::vec3 windAddition;
		float windClamp = 1.0f;
		float maxSpeed = 2.0f;
		
		if (wind.x > windClamp || wind.z > windClamp)
		{
			windAddition = glm::vec3(randInRange(-0.02f, 0.002f), 0.0f, randInRange(-0.02f, 0.002f));
		}
		else if (wind.x < -windClamp || wind.z < -windClamp)
		{
			windAddition = glm::vec3(randInRange(-0.002f, 0.02f), 0.0f, randInRange(-0.002f, 0.02f));
		}
		else
		{
			windAddition = glm::vec3(randInRange(-0.02f, 0.02f), 0.0f, randInRange(-0.02f, 0.02f));
		}
		wind = wind + windAddition;
		for (size_t idx = 0; idx < numNodes; idx++)
		{
			if (mNodes[idx]->Acceleration.x > maxSpeed || mNodes[idx]->Acceleration.z > maxSpeed || mNodes[idx]->Acceleration.x < -maxSpeed || mNodes[idx]->Acceleration.z < -maxSpeed)
			{
				wind = glm::vec3(0.0f);
			}
			mNodes[idx]->Acceleration += wind;
		}
		// step 2 - Accumulate spring forces based on current positions
		size_t numSprings = mSprings.size();
		for (size_t idx = 0; idx < numSprings; idx++)
		{
			mSprings[idx]->UpdateSpringForce();
			mSprings[idx]->ApplyForceToNodes();
		}
		// step 3 - Integrate nodes
		for (size_t idx = 0; idx < numNodes; idx++)
		{
			cNode* node = mNodes[idx];
			node->PreviousPosition = node->Position;
			if (node->IsFixed())
				continue;
			node->Velocity += node->Acceleration * dt;
			// dampen the velocity
			node->Velocity *= glm::pow(0.9f, dt);
			node->Position += node->Velocity * dt;
		}
		// step 4 - collide
		std::vector<std::pair<cNode*, cNode*>> collisions;
		for (size_t idxA = 0; idxA < numNodes - 1; idxA++)
		{
			for (size_t idxB = idxA + 1; idxB < numNodes; idxB++)
			{
				if (mNodes[idxA]->IsNeighbor(mNodes[idxB]))
				{
					continue;
				}
				if (Collide(mNodes[idxA], mNodes[idxB]))
				{
					collisions.push_back(std::make_pair(mNodes[idxA], mNodes[idxB]));
				}
			}
		}
	}
	bool cSoftBody::Collide(cNode* bodyA, cNode* bodyB)
	{
		if (bodyA->IsFixed() && bodyB->IsFixed())
		{
			return false;
		}

		//return CollideNodeNode(bodyA, bodyB);
	}
	bool cSoftBody::CollideSpherePlane(cRigidBody* thePlane, cPlane* theShape)
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

		std::vector<cNode*> closeNodes;
		glm::vec3 planePosition;
		thePlane->GetPosition(planePosition);
		glm::vec3 spherePreviousPosition;

		for (size_t idx = 0; idx < this->mNodes.size(); idx++)
		{
			float distanceSphereToNode = glm::distance(planePosition, mNodes[idx]->Position);
			float collisionDistance = mNodes[idx]->Radius;
			if (distanceSphereToNode < collisionDistance)
			{
				closeNodes.push_back(mNodes[idx]);
			}
		}

		for (int idx = 0; idx < closeNodes.size(); idx++)
		{

			glm::vec3 c = closeNodes[idx]->PreviousPosition;
			float r = closeNodes[idx]->Radius;
			glm::vec3 v = closeNodes[idx]->Position - closeNodes[idx]->PreviousPosition;
			glm::vec3 n = theShape->GetNormal();
			float d = theShape->GetConstant();
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

				glm::vec3 pointOnPlane = nCollide::closest_point_on_plane(closeNodes[idx]->PreviousPosition, theShape->GetNormal(), theShape->GetConstant());
				// figure out an impulse that should have the sphere escape the plane
				float distance = glm::distance(closeNodes[idx]->PreviousPosition, pointOnPlane);
				float targetDistance = r;
				glm::vec3 impulse = n * (targetDistance - distance) / 0.03f;
				//reset
				closeNodes[idx]->Position = closeNodes[idx]->PreviousPosition;
				//apply impulse
				closeNodes[idx]->Velocity += impulse;
				//re-do the timestep
				//IntegrateRigidBody(closeNodes[idx], 0.03f);
				this->Integrate(0.03f, glm::vec3(0.0f, -1.0f, 0.0f));

				//return true;
				//continue;
			}
			// it collided

			// reflect
			closeNodes[idx]->Velocity = glm::reflect(closeNodes[idx]->Velocity, theShape->GetNormal());
			// lose some energy
			glm::vec3 nComponent = glm::proj(closeNodes[idx]->Velocity, theShape->GetNormal());
			// rewind
			closeNodes[idx]->Velocity -= nComponent * 0.6f;

			closeNodes[idx]->Position = (c + v * t);
			//integrate
			//IntegrateRigidBody(closeNodes[idx], 0.03f * (1.0f - t));
			//finished
			
		}
		this->Integrate(0.03f, glm::vec3(0.0f, -1.0f, 0.0f));
		return true;
	}
	bool cSoftBody::CollideNodeNode(cNode* bodyA, cNode* bodyB)
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

		glm::vec3 cA = bodyA->PreviousPosition;
		glm::vec3 cB = bodyB->PreviousPosition;
		glm::vec3 vA = bodyA->Position - bodyA->PreviousPosition;
		glm::vec3 vB = bodyB->Position - bodyB->PreviousPosition;
		float rA = bodyA->Radius;
		float rB = bodyB->Radius;
		float t(0.0f);

		int result = nCollide::intersect_moving_sphere_sphere(cA, rA, vA, cB, rB, vB, t);
		if (result == 0)
		{
			// no collision
			return false;
		}

		// get the masses
		float ma = bodyA->Mass;
		float mb = bodyB->Mass;
		float mt = ma + mb;

		if (result == -1)
		{
			// already colliding

			float initialDistance = glm::distance(bodyA->PreviousPosition, bodyB->PreviousPosition);
			float targetDistance = rA + rB;

			glm::vec3 impulseToA = glm::normalize(bodyA->PreviousPosition - bodyB->PreviousPosition);
			impulseToA *= (targetDistance - initialDistance);

			// back to ones
			bodyA->Position = bodyA->PreviousPosition;
			bodyB->Position = bodyB->PreviousPosition;
			// apply the impulse
			bodyA->Velocity += impulseToA * (mb / mt);
			bodyB->Velocity -= impulseToA * (ma / mt);

			//integrate
			
			//IntegrateRigidBody(bodyA, mDeltaTime);
			//IntegrateRigidBody(bodyB, mDeltaTime);

			return true;
		}

		// collided

		// everybody to ones
		// rewind to point of collision
		bodyA->Position = bodyA->PreviousPosition + vA * t;
		bodyB->Position = bodyB->PreviousPosition + vB * t;

		vA = bodyA->Velocity;
		vB = bodyB->Velocity;

		float c = 0.2f;
		bodyA->Velocity = (c * mb * (vB - vA) + ma * vA + mb * vB) / mt;
		bodyB->Velocity = (c * ma * (vA - vB) + ma * vA + mb * vB) / mt;

		// integrate
		//IntegrateRigidBody(bodyA, mDeltaTime * (1.0f - t));
		//IntegrateRigidBody(bodyB, mDeltaTime * (1.0f - t));

		return true;
	}


	bool cSoftBody::CollideSphereCloth(cRigidBody* sphere)
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

		std::vector<cNode*> closeNodes;
		glm::vec3 spherePosition;
		sphere->GetPosition(spherePosition);
		glm::vec3 spherePreviousPosition;
		sphere->GetPreviousPosition(spherePreviousPosition);
		float sphereRadius;
		sphere->GetRadius(sphereRadius);
		float sphereMass;
		sphere->GetMass(sphereMass);
		glm::vec3 sphereVelocity;
		sphere->GetVelocity(sphereVelocity);

		for (size_t idx = 0; idx < this->mNodes.size(); idx++)
		{			
			float distanceSphereToNode = glm::distance(spherePosition, mNodes[idx]->Position);
			float collisionDistance = sphereRadius + mNodes[idx]->Radius;
			if (distanceSphereToNode < collisionDistance)
			{
				closeNodes.push_back(mNodes[idx]);
			}
		}

		for (size_t idx = 0; idx < closeNodes.size(); idx++)
		{
			glm::vec3 cA = spherePreviousPosition;
			glm::vec3 cB = closeNodes[idx]->PreviousPosition;
			glm::vec3 vA = spherePosition - spherePreviousPosition;
			glm::vec3 vB = closeNodes[idx]->Position - closeNodes[idx]->PreviousPosition;
			float rA = sphereRadius;
			float rB = closeNodes[idx]->Radius;
			float t(0.0f);

			int result = nCollide::intersect_moving_sphere_sphere(cA, rA, vA, cB, rB, vB, t);
			if (result == 0)
			{
				// no collision
				continue;
			}

			// get the masses
			
			float ma = sphereMass;
			float mb = closeNodes[idx]->Mass;
			float mt = ma + mb;

			if (result == -1)
			{
				// already colliding

				float initialDistance = glm::distance(spherePreviousPosition, closeNodes[idx]->PreviousPosition);
				float targetDistance = rA + rB;

				glm::vec3 impulseToA = glm::normalize(spherePreviousPosition - closeNodes[idx]->PreviousPosition);
				impulseToA *= (targetDistance - initialDistance);

				// just push the cloth out of the way
				//bodyA->mPosition = bodyA->mPreviousPosition;
				closeNodes[idx]->Position = closeNodes[idx]->PreviousPosition;
				// apply the impulse
				//bodyA->mVelocity += impulseToA * (mb / mt);
				closeNodes[idx]->Velocity -= impulseToA * (ma / mt);

				//integrate
				//IntegrateRigidBody(bodyA, mDeltaTime);
				//IntegrateRigidBody(bodyB, mDeltaTime);
				//this->Integrate(0.03f, glm::vec3(0.0f, -1.0f, 0.0f));

				//return true;
				continue;
			}

			// collided

			// everybody to ones
			// just push the cloth
			//bodyA->mPosition = bodyA->mPreviousPosition + vA * t;
			closeNodes[idx]->Position = closeNodes[idx]->PreviousPosition + vB * t;

			vA = sphereVelocity;
			vB = closeNodes[idx]->Velocity;

			float c = 0.2f;
			//bodyA->mVelocity = (c * mb * (vB - vA) + ma * vA + mb * vB) / mt;
			closeNodes[idx]->Velocity = ((c * ma * (vA - vB) + ma * vA + mb * vB)) / mt;

			// integrate
			//IntegrateRigidBody(bodyA, mDeltaTime * (1.0f - t));
			//IntegrateRigidBody(bodyB, mDeltaTime * (1.0f - t));
			
		}
		this->Integrate(0.03f, glm::vec3(0.0f, -1.0f, 0.0f));
		return true;
	}
	size_t cSoftBody::NumNodes()
	{
		return mNodes.size();
	}
	bool cSoftBody::GetNodeRadius(size_t index, float& radiusOut)
	{
		if (index >= mNodes.size())
			return false;
		radiusOut = mNodes[index]->Radius;
		return true;
	}
	bool cSoftBody::GetNodePosition(size_t index, glm::vec3& positionOut)
	{
		if (index >= mNodes.size())
			return false;
		positionOut = mNodes[index]->Position;
		return true;
	}
	void cSoftBody::IntegrateNode(cNode* node)
	{
		if (node->IsFixed()) return;
		// TODO: You can do it! I believe in you!
	}
}