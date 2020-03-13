#include "cSoftBody.h"
#include <numeric>
#include "nCollide.h"

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
	cSoftBody::cNode::cNode(const sSoftBodyNodeDef& nodeDef) : Position(nodeDef.Position), Mass(nodeDef.Mass), Velocity(0.0f,0.0f,0.0f), Acceleration(0.0f,0.0f,0.0f), Radius(1.0f)
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
	void cSoftBody::Integrate(float dt, const glm::vec3& gravity)
	{
		size_t numNodes = mNodes.size();
		// step 1 - gravity
		for (size_t idx = 0; idx < numNodes; idx++)
		{			
			mNodes[idx]->Acceleration = gravity;
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

		return CollideNodeNode(bodyA, bodyB);
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