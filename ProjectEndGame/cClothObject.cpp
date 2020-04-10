#include "cClothObject.h"

void cClothObject::Shoot()
{
	this->pShooter->Shoot();
}

std::vector<glm::vec3> cClothObject::getTestPoints()
{
	return this->testPoints;
}

std::string cClothObject::getTextures(int index)
{
	return this->textures[index];
}

float cClothObject::getTextureRatio(int index)
{
	return this->textureRatio[index];
}

float cClothObject::getTransprancyValue()
{
	return this->transprancyValue;
}

bool cClothObject::getIsTranspanrancy()
{
	return this->isTranspanrancy;
}

cSimpleAssimpSkinnedMesh* cClothObject::getSM()
{
	return this->pSM;
}

std::string cClothObject::getAnimation()
{
	return std::string();
}

std::string cClothObject::getBehaviour()
{
	return this->_Behaviour;
}

std::string cClothObject::getMeshName()
{
	return this->_meshName;
}
unsigned int cClothObject::getFriendlyIDNumber()
{
	return this->_friendlyIDNumber;
}
std::string cClothObject::getFriendlyName()
{
	return this->_friendlyName;
}
glm::vec3 cClothObject::getPositionXYZ()
{
	glm::vec3 result = glm::vec3(0.0f);
	if (this->GetComponent() != nullptr)
	{
		this->_component->GetPosition(result);
	}
	else
	{
		result = _positionXYZ;
	}
	return result;
}
glm::quat cClothObject::getRotationXYZ()
{
	return this->qRotation;
}
float cClothObject::getScale()
{
	return this->_scale;
}
glm::mat4 cClothObject::getMatWorld()
{
	return this->_matWorld;
}
glm::vec4 cClothObject::getObjectColourRGBA()
{
	return this->_objectColourRGBA;
}
glm::vec4 cClothObject::getDiffuseColour()
{
	return this->_diffuseColour;
}
glm::vec4 cClothObject::getSpecularColour()
{
	return this->_specularColour;
}
glm::vec3 cClothObject::getVelocity()
{
	glm::vec3 theVelocity = glm::vec3(0.0f);
	if (this->_component != nullptr)
	{
		this->_component->GetVelocity(theVelocity);
	}
	else
	{
		theVelocity = _velocity;
	}
	return theVelocity;
}
float cClothObject::getVelocityX()
{
	return this->_velocityX;
}
float cClothObject::getVelocityY()
{
	return this->_velocityY;
}
float cClothObject::getVelocityZ()
{
	return this->_velocityZ;
}
glm::vec3 cClothObject::getAccel()
{
	return this->_accel;
}
float cClothObject::getInverseMass()
{
	return this->_inverseMass;
}
std::string cClothObject::getPhysicsShapeType()
{
	return this->_physicsShapeType;
}
bool cClothObject::getIsWireframe()
{
	return this->_isWireframe;
}
glm::vec4 cClothObject::getDebugColour()
{
	return this->_debugColour;
}
float cClothObject::get_SPHERE_radius()
{
	return this->_SPHERE_radius;
}
bool cClothObject::getIsVisible()
{
	return this->_isVisible;
}
bool cClothObject::getDisableDepthBufferTest()
{
	return this->_disableDepthBufferTest;
}
bool cClothObject::getDisableDepthBufferWrite()
{
	return this->_disableDepthBufferWrite;
}
//setters

void cClothObject::addTestPoint(glm::vec3 testPoint)
{
	this->testPoints.push_back(testPoint);
}
void cClothObject::setAnimation(std::string animation)
{
}
void cClothObject::setBehaviour(std::string behaviour)
{
	this->_Behaviour = behaviour;
}
void cClothObject::setSM(cSimpleAssimpSkinnedMesh* skinnedMesh)
{
	this->pSM = skinnedMesh;
}
void cClothObject::setMeshName(std::string name)
{
	this->_meshName = name;
}
void cClothObject::setFriendlyIDNumber(unsigned int IDNumber)
{
	this->_friendlyIDNumber = IDNumber;
}
void cClothObject::setFriendlyName(std::string friendlyName)
{
	this->_friendlyName = friendlyName;
}
void cClothObject::setPositionXYZ(glm::vec3 positionXYZ)
{
	if (this->_component != NULL)
	{
		this->_component->SetPosition(positionXYZ);
	}
	else
		this->_positionXYZ = positionXYZ;
}
void cClothObject::setRotationXYZ(glm::quat rotationXYZ)
{
	this->qRotation = rotationXYZ;
}
void cClothObject::setScale(float scale)
{
	this->_scale = scale;
}
void cClothObject::setMatWorld(glm::mat4 matWorld)
{
	this->_matWorld = matWorld;
}
void cClothObject::setObjectColourRGBA(glm::vec4 colourRGBA)
{
	this->_objectColourRGBA = colourRGBA;
}
void cClothObject::setDiffuseColour(glm::vec4 diffuseColourRGBA)
{
	this->_diffuseColour = diffuseColourRGBA;
}
void cClothObject::setSpecularColour(glm::vec4 specularColourRGBA)
{
	this->_specularColour = specularColourRGBA;
}
void cClothObject::setVelocity(glm::vec3 velocityXYZ)
{
	this->_velocity = velocityXYZ;
}
void cClothObject::setVelocityX(float velocityX)
{
	this->_velocityX = velocityX;
}
void cClothObject::setVelocityY(float velocityY)
{
	this->_velocityY = velocityY;
}
void cClothObject::setVelocityZ(float velocityZ)
{
	this->_velocityZ = velocityZ;
}
void cClothObject::setAccel(glm::vec3 accelXYZ)
{
	this->_accel = accelXYZ;
}
void cClothObject::setInverseMass(float inverseMass)
{
	this->_inverseMass = inverseMass;
}
void cClothObject::setPhysicsShapeType(std::string physicsShapeType)
{
	this->_physicsShapeType = physicsShapeType;
}
void cClothObject::setIsWireframe(bool isWireFrame)
{
	this->_isWireframe = isWireFrame;
}
void cClothObject::setDebugColour(glm::vec4 debugColourRGBA)
{
	this->_debugColour = debugColourRGBA;
}
void cClothObject::set_SPHERE_radius(float radius)
{
	this->_SPHERE_radius = radius;
}
void cClothObject::setIsVisible(bool isVisible)
{
	this->_isVisible = isVisible;
}
void cClothObject::setDisableDepthBufferTest(bool disableDepthBufferTest)
{
	this->_disableDepthBufferTest = disableDepthBufferTest;
}
void cClothObject::setDisableDepthBufferWrite(bool disableDepthBufferWrite)
{
	this->_disableDepthBufferWrite = disableDepthBufferWrite;
}

void cClothObject::GetTransform(glm::mat4& transformOut)
{
	transformOut = this->_matWorld;
}

void cClothObject::GetPosition(glm::vec3& positionOut)
{
	glm::vec3 result = glm::vec3(0.0f, 0.0f, 0.0f);
	//this->_component->GetPosition(result);
	positionOut = result;
}

void cClothObject::SetPosition(glm::vec3 positionIn)
{
	//this->_component->SetPosition(positionIn);
}

void cClothObject::GetVelocity(glm::vec3& velocityOut)
{
	velocityOut = this->_velocity;
}

void cClothObject::ApplyForce(const glm::vec3& force)
{
	//this->_component->ApplyForce(force);
}

int cClothObject::GetMassType()
{
	return _physicsType;
}

void cClothObject::SetMassType(int physicsType)
{
	_physicsType = physicsType;
}

std::string cClothObject::GetPlaneType()
{
	return this->_planeType;
}

void cClothObject::SetPlaneType(std::string planeType)
{
	this->_planeType = planeType;
}

void cClothObject::SetComponent(nPhysics::iPhysicsComponent* component)
{
	this->_component = component;
}

nPhysics::iPhysicsComponent* cClothObject::GetComponent()
{
	return this->_component;
}

cClothObject::cClothObject(nPhysics::eComponentType componentType)
{
	this->_scale = 0.0f;
	this->_isVisible = true;
	this->mComponentType = componentType;

	this->_isWireframe = false;
	this->_debugColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	this->_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	this->_accel = glm::vec3(0.0f, 0.0f, 0.0f);
	this->_inverseMass = 0.0f;	// Infinite mass
	this->_physicsShapeType = "UNKNOWN";

	// Set the unique ID
	// Take the value of the static int, 
	//  set this to the instance variable
	this->_m_uniqueID = cClothObject::next_uniqueID;
	// Then increment the static variable
	cClothObject::next_uniqueID++;

	this->_disableDepthBufferTest = false;
	this->_disableDepthBufferWrite = false;


	return;
}

void cClothObject::MoveInRelativeDirection(glm::vec3 relativeDirection)
{
	// The "forward" vector is +ve Z
	// (the 4th number is because we need a vec4 later)
	glm::vec4 forwardDirObject = glm::vec4(relativeDirection, 1.0f);

	glm::mat4 matModel = glm::mat4(1.0f);	// Identity

	// Roation
	// Constructor for the GLM mat4x4 can take a quaternion
	glm::mat4 matRotation = glm::mat4(this->getRotationXYZ());
	matModel *= matRotation;
	// *******************

	// Like in the vertex shader, I mulitly the test points
	// by the model matrix (MUST be a VEC4 because it's a 4x4 matrix)
	glm::vec4 forwardInWorldSpace = matModel * forwardDirObject;


	// Add this to the position of the object
	this->setPositionXYZ(this->getPositionXYZ() + glm::vec3(forwardInWorldSpace));

	return;
}

void cClothObject::setTexture(std::string tex, int index)
{
	this->textures[index] = tex;
}

void cClothObject::setTextureRatio(float texRatio, int index)
{
	this->textureRatio[index] = texRatio;
}

void cClothObject::setTransprancyValue(float transprancyValue)
{
	this->transprancyValue = transprancyValue;
}

void cClothObject::setIsTranspanrancy(bool isTranspanrancy)
{
	this->isTranspanrancy = isTranspanrancy;
}


unsigned int cClothObject::getUniqueID(void)
{
	return this->_m_uniqueID;
}

// this variable is static, so common to all objects.
// When the object is created, the unique ID is set, and 
//	the next unique ID is incremented
//static 
unsigned int cClothObject::next_uniqueID = 1000;	// Starting at 1000, just because
