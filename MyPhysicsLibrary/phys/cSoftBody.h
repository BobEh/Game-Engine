#pragma once
#include "iShape.h"     // iShape base class for all shapes
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include <physics\interfaces\iRigidBody.h>
#include <vector>
#include "cCollisionBody.h"
#include "cRigidBody.h"
#include "shapes.h"

namespace phys
{
	struct sSoftBodyNodeDef
	{
		glm::vec3 Position;
		float Mass;
		float Radius;
	};
	struct sSoftBodyDef
	{
		std::vector<sSoftBodyNodeDef> Nodes;
		std::vector<std::pair<size_t, size_t>> Springs;
		float SpringConstant;
	};

	class cSoftBody : public cCollisionBody
	{
	private:
		class cSpring;
		class cNode
		{
		public:
			cNode(const sSoftBodyNodeDef& nodeDef);
			inline bool IsFixed() { return Mass == 0.0f; }
			std::vector<cSpring*> Springs;
			bool IsNeighbor(cNode* other);
			float Radius;
			float Mass;
			glm::vec3 Position;
			glm::vec3 PreviousPosition;
			glm::vec3 Velocity;
			glm::vec3 Acceleration;

			void CalculateRadius();
		};
		class cSpring
		{
		public:
			cSpring(cNode* nodeA, cNode* nodeB, float springConstant);
			cNode* NodeA;
			cNode* NodeB;
			cNode* GetOther(cNode* node);
			float SpringConstant;
			glm::vec3 SpringForceAtoB;
			float RestingLength;

			void UpdateSpringForce();
			void ApplyForceToNodes();
		};
	public:
		cSoftBody(const sSoftBodyDef& def);
		virtual ~cSoftBody();

		virtual void ClearAccelerations();
		void Integrate(float deltaTime, const glm::vec3& gravity);
		bool Collide(cNode* bodyA, cNode* bodyB);
		bool CollideNodeNode(cNode* bodyA, cNode* bodyB);
		bool CollideSpherePlane(cRigidBody* thePlane, cPlane* theShape);
		bool CollideSphereCloth(cRigidBody* theSphere);

		size_t NumNodes();
		bool GetNodeRadius(size_t index, float& radiusOut);
		bool GetNodePosition(size_t index, glm::vec3& positionOut);


	protected:
		void IntegrateNode(cNode* node);

	private:
		std::vector<cNode*> mNodes;
		std::vector<cSpring*> mSprings;
	};
}