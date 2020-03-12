#include "cSoftBody.h"
#include <numeric>

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
		SpringForceAtoB = glm::normalize(sep) * x * SpringConstant;
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