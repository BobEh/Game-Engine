#include "cClothComponent.h"

namespace nPhysics
{
	cClothComponent::cClothComponent(const sClothDef& theClothDef) :
		mBody(0)
	{
		size_t numNodes = theClothDef.NumNodesAcross * theClothDef.NumNodesDown;
		phys::sSoftBodyDef physDef;

		physDef.SpringConstant = theClothDef.SpringConstant;

		physDef.Nodes.resize(numNodes);
		size_t idxNode = 0;

		glm::vec3 sepAcross(theClothDef.CornerB - theClothDef.CornerA);
		sepAcross /= (theClothDef.NumNodesAcross - 1);

		glm::vec3 sepDown(glm::normalize(theClothDef.DownDirection));
		sepDown *= glm::length(sepAcross);

		for (size_t idxDown = 0; idxDown < theClothDef.NumNodesDown; idxDown++)		
		{
			for (size_t idxAcross = 0; idxAcross < theClothDef.NumNodesAcross; idxAcross++)
			{
				
				physDef.Nodes[idxNode].Position = theClothDef.CornerA + sepAcross * (float)idxAcross - sepDown * (float)idxDown;
				physDef.Nodes[idxNode].Mass = theClothDef.NodeMass;
				physDef.Nodes[idxNode].Radius = theClothDef.NodeRadius;
				idxNode++;
			}
		}
		// top row mass to zero
		for (size_t idxAcross = 0; idxAcross < theClothDef.NumNodesAcross; idxAcross++)
		{
			physDef.Nodes[idxAcross].Mass = 0.0f;
		}
		// spring info
		for (size_t idxDown = 0; idxDown < theClothDef.NumNodesDown - 1; idxDown++)		
		{
			for (size_t idxAcross = 0; idxAcross < theClothDef.NumNodesAcross - 1; idxAcross++)
			{
				idxNode = (idxDown * theClothDef.NumNodesAcross) + idxAcross;
				// set the across spring, going right
				physDef.Springs.push_back(std::make_pair(idxNode, idxNode + 1));
				// set the down spring, going down
				physDef.Springs.push_back(std::make_pair(idxNode, idxNode + theClothDef.NumNodesAcross));
			}
		}
		// set the bottom row springs
		size_t idxBottomRowStart = theClothDef.NumNodesAcross * (theClothDef.NumNodesDown - 1);
		for (size_t idxAcross = 0; idxAcross < theClothDef.NumNodesAcross - 1; idxAcross++)
		{
			// set the bottom row spring going right
			physDef.Springs.push_back(std::make_pair(idxBottomRowStart + idxAcross, idxBottomRowStart + idxAcross + 1));
		}
		// set the rightmost column
		for (size_t idxDown = 0; idxDown < theClothDef.NumNodesDown - 1; idxDown++)
		{
			idxNode = theClothDef.NumNodesAcross * (idxDown + 1) - 1;
			// set the rightmost column spring going down
			physDef.Springs.push_back(std::make_pair(idxNode, idxNode + theClothDef.NumNodesAcross));
		}
		for (size_t idx = 0; idx < physDef.Springs.size(); idx++)
		{

		}
		mBody = new phys::cSoftBody(physDef);
	}
	cClothComponent::~cClothComponent()
	{
		if (mBody)
		{
			delete mBody;
			mBody = 0;
		}
	}
	void cClothComponent::ApplyForce(const glm::vec3& force)
	{
	}
	void cClothComponent::GetTransform(glm::mat4& transformOut)
	{
		transformOut = glm::mat4(1.0f);
	}
	size_t cClothComponent::NumNodes()
	{
		return mBody->NumNodes();
	}
	bool cClothComponent::GetNodeRadius(size_t index, float& radiusOut)
	{
		return mBody->GetNodeRadius(index, radiusOut);
	}
	bool cClothComponent::GetNodePosition(size_t index, glm::vec3& positionOut)
	{
		return mBody->GetNodePosition(index, positionOut);
	}
	void cClothComponent::GetPosition(glm::vec3& positionOut)
	{
	}
	void cClothComponent::SetPosition(glm::vec3 positionIn)
	{
	}
	void cClothComponent::GetVelocity(glm::vec3& velocityOut)
	{
	}
	void cClothComponent::GetScale(float& scaleOut)
	{
		scaleOut = 1.0f;
	}
	int cClothComponent::GetMassType()
	{
		return 0;
	}
	void cClothComponent::SetMassType(int physicsType)
	{
	}
	std::string cClothComponent::GetPlaneType()
	{
		return std::string();
	}
}