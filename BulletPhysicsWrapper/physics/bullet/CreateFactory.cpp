#include "CreateFactory.h"

DLL_EXPORT nPhysics::iPhysicsFactory* createFactory()
{
	return new nPhysics::cPhysicsFactory();
}
