#include "cDebugRenderer.h"
#include <nConvert.h>

namespace nPhysics
{
	cDebugRenderer::cDebugRenderer(iDebugRenderer* debugRenderer)
	{
		mDebugRenderer = debugRenderer;
	}
	cDebugRenderer::~cDebugRenderer()
	{
		mDebugRenderer = 0;
	}
	void cDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		// TODO: draw lines
		//mDebugRenderer->DrawLine(nConvert::ToSimple(from), nConvert::ToSimple(to), nConvert::ToSimple(color));
	}
	void cDebugRenderer::reportErrorWarning(const char* warningString)
	{

	}
	void cDebugRenderer::draw3dText(const btVector3& location, const char* textString)
	{

	}
	void cDebugRenderer::setDebugMode(int debugMode)
	{

	}
	int cDebugRenderer::getDebugMode() const
	{
		return 0; // TODO: this
	}
}