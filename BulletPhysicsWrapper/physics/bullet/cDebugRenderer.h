#pragma once
#include <physics/interfaces/iDebugRenderer.h>
#include "btBulletDynamicsCommon.h"

namespace nPhysics
{
	class cDebugRenderer : public btIDebugDraw
	{
	public:
		cDebugRenderer(iDebugRenderer* debugRenderer);
		virtual ~cDebugRenderer();
		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
		virtual void reportErrorWarning(const char* warningString);
		virtual void draw3dText(const btVector3& location, const char* textString);
		virtual void setDebugMode(int debugMode);
		virtual int getDebugMode() const;

	private:
		iDebugRenderer* mDebugRenderer;
	};
}