#include "cObjectFactory.h"
#include "cGameObject.h"
#include "cSphereObject.h"
#include "cClothObject.h"
#include "cMeshObject.h"


iObject* cObjectFactory::CreateObject(std::string objectType, nPhysics::eComponentType componentType)
{
	iObject* pTheObject = NULL;		// 0

	if (objectType == "unknown")
	{
		pTheObject = new cGameObject(componentType);
	}
	else if (objectType == "sphere")
	{
		pTheObject = new cSphereObject(componentType);
	}
	else if (objectType == "mesh")
	{
		pTheObject = new cMeshObject(componentType);
	}
	else if (objectType == "cloth")
	{
		pTheObject = new cClothObject(componentType);
	}

	return pTheObject;
}

void cObjectFactory::BuildObject(iObject* pObject, std::string objectType)
{
	if (objectType == "shpere")
	{
		((cSphereObject*)pObject)->pShooter = new cShooter();
	}
	else if (objectType == "mesh")
	{
		((cMeshObject*)pObject)->pBigShooter = new cBigShooter();
	}

}