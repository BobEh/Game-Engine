#include "cSphereObject.h"

void cSphereObject::Shoot()
{
	this->pShooter->Shoot();
}

std::vector<glm::vec3> cSphereObject::getTestPoints()
{
	EnterCriticalSection(&object_lock);
	std::vector<glm::vec3> result = this->testPoints;
	LeaveCriticalSection(&object_lock);
	return result;
}

std::string cSphereObject::getTextures(int index)
{
	EnterCriticalSection(&object_lock);
	std::string result = this->textures[index];
	LeaveCriticalSection(&object_lock);
	return result;
}

float cSphereObject::getTextureRatio(int index)
{
	EnterCriticalSection(&object_lock);
	float result = this->textureRatio[index];
	LeaveCriticalSection(&object_lock);
	return result;
}

float cSphereObject::getTransprancyValue()
{
	EnterCriticalSection(&object_lock);
	float result = this->transprancyValue;
	LeaveCriticalSection(&object_lock);
	return result;
}

bool cSphereObject::getIsTranspanrancy()
{
	EnterCriticalSection(&object_lock);
	bool result = this->isTranspanrancy;
	LeaveCriticalSection(&object_lock);
	return result;
}

cSimpleAssimpSkinnedMesh* cSphereObject::getSM()
{
	EnterCriticalSection(&object_lock);
	cSimpleAssimpSkinnedMesh* result = this->pSM;
	LeaveCriticalSection(&object_lock);
	return result;
}

std::string cSphereObject::getAnimation()
{
	return this->_Animation;
}

std::string cSphereObject::getBehaviour()
{
	EnterCriticalSection(&object_lock);
	std::string result = this->_Behaviour;
	LeaveCriticalSection(&object_lock);
	return result;
}

std::string cSphereObject::getMeshName()
{
	EnterCriticalSection(&object_lock);
	std::string result = this->_meshName;
	LeaveCriticalSection(&object_lock);
	return result;
}
unsigned int cSphereObject::getFriendlyIDNumber()
{
	EnterCriticalSection(&object_lock);
	unsigned int result = this->_friendlyIDNumber;
	LeaveCriticalSection(&object_lock);
	return result;
}
std::string cSphereObject::getFriendlyName()
{
	EnterCriticalSection(&object_lock);
	std::string result = this->_friendlyName;
	LeaveCriticalSection(&object_lock);
	return result;
}
glm::vec3 cSphereObject::getPositionXYZ()
{
	glm::vec3 result = glm::vec3(0.0f);
	if (this->_component != nullptr)
	{
		this->_component->GetPosition(result);
	}
	else
	{
		EnterCriticalSection(&object_lock);
		result = _positionXYZ;
		LeaveCriticalSection(&object_lock);
	}
	return result;
}
glm::quat cSphereObject::getRotationXYZ()
{
	//glm::quat result = glm::quat(glm::vec3(0.0f));
	//EnterCriticalSection(&object_lock);
	//result = this->qRotation;
	//LeaveCriticalSection(&object_lock);
	return glm::quat(this->qRotation);
}

float cSphereObject::getScale()
{
	float scale = 1.0f;
	if (this->_component != nullptr)
	{
		this->GetComponent()->GetScale(scale);
	}
	else
	{
		EnterCriticalSection(&object_lock);
		scale = _scale;
		LeaveCriticalSection(&object_lock);
	}
	return scale;
}
glm::mat4 cSphereObject::getMatWorld()
{
	EnterCriticalSection(&object_lock);
	glm::mat4 result = this->_matWorld;
	LeaveCriticalSection(&object_lock);
	return result;
}
glm::vec4 cSphereObject::getObjectColourRGBA()
{
	EnterCriticalSection(&object_lock);
	glm::vec4 result = this->_objectColourRGBA;
	LeaveCriticalSection(&object_lock);
	return result;
}
glm::vec4 cSphereObject::getDiffuseColour()
{
	EnterCriticalSection(&object_lock);
	glm::vec4 result = this->_diffuseColour;
	LeaveCriticalSection(&object_lock);
	return result;
}
glm::vec4 cSphereObject::getSpecularColour()
{
	EnterCriticalSection(&object_lock);
	glm::vec4 result = this->_specularColour;
	LeaveCriticalSection(&object_lock);
	return result;
}
glm::vec3 cSphereObject::getVelocity()
{
	glm::vec3 theVelocity = glm::vec3(0.0f);
	if (this->_component != nullptr)
	{
		this->_component->GetVelocity(theVelocity);
	}
	else
	{
		EnterCriticalSection(&object_lock);
		theVelocity = _velocity;
		LeaveCriticalSection(&object_lock);
	}
	return theVelocity;
}
float cSphereObject::getVelocityX()
{
	EnterCriticalSection(&object_lock);
	float result = this->_velocityX;
	LeaveCriticalSection(&object_lock);
	return result;
}
float cSphereObject::getVelocityY()
{
	EnterCriticalSection(&object_lock);
	float result = this->_velocityY;
	LeaveCriticalSection(&object_lock);
	return result;
}
float cSphereObject::getVelocityZ()
{
	EnterCriticalSection(&object_lock);
	float result = this->_velocityZ;
	LeaveCriticalSection(&object_lock);
	return result;
}
glm::vec3 cSphereObject::getAccel()
{
	EnterCriticalSection(&object_lock);
	glm::vec3 result = this->_accel;
	LeaveCriticalSection(&object_lock);
	return result;
}
float cSphereObject::getInverseMass()
{
	EnterCriticalSection(&object_lock);
	float result = this->_inverseMass;
	LeaveCriticalSection(&object_lock);
	return result;
}
std::string cSphereObject::getPhysicsShapeType()
{
	EnterCriticalSection(&object_lock);
	std::string result = this->_physicsShapeType;
	LeaveCriticalSection(&object_lock);
	return result;
}
bool cSphereObject::getIsWireframe()
{
	EnterCriticalSection(&object_lock);
	bool result = this->_isWireframe;
	LeaveCriticalSection(&object_lock);
	return result;
}
glm::vec4 cSphereObject::getDebugColour()
{
	EnterCriticalSection(&object_lock);
	glm::vec4 result = this->_debugColour;
	LeaveCriticalSection(&object_lock);
	return result;
}
float cSphereObject::get_SPHERE_radius()
{
	EnterCriticalSection(&object_lock);
	float result = this->_SPHERE_radius;
	LeaveCriticalSection(&object_lock);
	return result;
}
bool cSphereObject::getIsVisible()
{
	EnterCriticalSection(&object_lock);
	bool result = this->_isVisible;
	LeaveCriticalSection(&object_lock);
	return result;
}
bool cSphereObject::getDisableDepthBufferTest()
{
	EnterCriticalSection(&object_lock);
	bool result = this->_disableDepthBufferTest;
	LeaveCriticalSection(&object_lock);
	return result;
}
bool cSphereObject::getDisableDepthBufferWrite()
{
	EnterCriticalSection(&object_lock);
	bool result = this->_disableDepthBufferWrite;
	LeaveCriticalSection(&object_lock);
	return result;
}
//setters

void cSphereObject::addTestPoint(glm::vec3 testPoint)
{
	EnterCriticalSection(&object_lock);
	this->testPoints.push_back(testPoint);
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setAnimation(std::string animation)
{
	this->_Animation = animation;
}
void cSphereObject::setBehaviour(std::string behaviour)
{
	EnterCriticalSection(&object_lock);
	this->_Behaviour = behaviour;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setSM(cSimpleAssimpSkinnedMesh* skinnedMesh)
{
	EnterCriticalSection(&object_lock);
	this->pSM = skinnedMesh;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setMeshName(std::string name)
{
	EnterCriticalSection(&object_lock);
	this->_meshName = name;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setFriendlyIDNumber(unsigned int IDNumber)
{
	EnterCriticalSection(&object_lock);
	this->_friendlyIDNumber = IDNumber;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setFriendlyName(std::string friendlyName)
{
	EnterCriticalSection(&object_lock);
	this->_friendlyName = friendlyName;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setPositionXYZ(glm::vec3 positionXYZ)
{
	if (this->_component != NULL)
	{
		this->_component->SetPosition(positionXYZ);
	}
	else
	{
		EnterCriticalSection(&object_lock);
		this->_positionXYZ = positionXYZ;
		LeaveCriticalSection(&object_lock);
	}
}
void cSphereObject::setRotationXYZ(glm::quat rotationXYZ)
{
	EnterCriticalSection(&object_lock);
	this->qRotation = rotationXYZ;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setScale(float scale)
{
	EnterCriticalSection(&object_lock);
	this->_scale = scale;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setMatWorld(glm::mat4 matWorld)
{
	EnterCriticalSection(&object_lock);
	this->_matWorld = matWorld;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setObjectColourRGBA(glm::vec4 colourRGBA)
{
	EnterCriticalSection(&object_lock);
	this->_objectColourRGBA = colourRGBA;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setDiffuseColour(glm::vec4 diffuseColourRGBA)
{
	EnterCriticalSection(&object_lock);
	this->_diffuseColour = diffuseColourRGBA;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setSpecularColour(glm::vec4 specularColourRGBA)
{
	EnterCriticalSection(&object_lock);
	this->_specularColour = specularColourRGBA;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setVelocity(glm::vec3 velocityXYZ)
{
	EnterCriticalSection(&object_lock);
	this->_velocity = velocityXYZ;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setVelocityX(float velocityX)
{
	EnterCriticalSection(&object_lock);
	this->_velocityX = velocityX;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setVelocityY(float velocityY)
{
	EnterCriticalSection(&object_lock);
	this->_velocityY = velocityY;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setVelocityZ(float velocityZ)
{
	EnterCriticalSection(&object_lock);
	this->_velocityZ = velocityZ;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setAccel(glm::vec3 accelXYZ)
{
	EnterCriticalSection(&object_lock);
	this->_accel = accelXYZ;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setInverseMass(float inverseMass)
{
	EnterCriticalSection(&object_lock);
	this->_inverseMass = inverseMass;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setPhysicsShapeType(std::string physicsShapeType)
{
	EnterCriticalSection(&object_lock);
	this->_physicsShapeType = physicsShapeType;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setIsWireframe(bool isWireFrame)
{
	EnterCriticalSection(&object_lock);
	this->_isWireframe = isWireFrame;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setDebugColour(glm::vec4 debugColourRGBA)
{
	EnterCriticalSection(&object_lock);
	this->_debugColour = debugColourRGBA;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::set_SPHERE_radius(float radius)
{
	EnterCriticalSection(&object_lock);
	this->_SPHERE_radius = radius;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setIsVisible(bool isVisible)
{
	EnterCriticalSection(&object_lock);
	this->_isVisible = isVisible;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setDisableDepthBufferTest(bool disableDepthBufferTest)
{
	EnterCriticalSection(&object_lock);
	this->_disableDepthBufferTest = disableDepthBufferTest;
	LeaveCriticalSection(&object_lock);
}
void cSphereObject::setDisableDepthBufferWrite(bool disableDepthBufferWrite)
{
	EnterCriticalSection(&object_lock);
	this->_disableDepthBufferWrite = disableDepthBufferWrite;
	LeaveCriticalSection(&object_lock);
}

void cSphereObject::GetTransform(glm::mat4& transformOut)
{
	EnterCriticalSection(&object_lock);
	transformOut = this->_matWorld;
	LeaveCriticalSection(&object_lock);
}

void cSphereObject::GetPosition(glm::vec3& positionOut)
{
	EnterCriticalSection(&object_lock);
	glm::vec3 result = glm::vec3(0.0f, 0.0f, 0.0f);
	//this->_component->GetPosition(result);
	positionOut = result;
	LeaveCriticalSection(&object_lock);
}

void cSphereObject::SetPosition(glm::vec3 positionIn)
{
	//this->_component->SetPosition(positionIn);
}

void cSphereObject::GetVelocity(glm::vec3& velocityOut)
{
	glm::vec3 result = glm::vec3(0.0f);
	this->_component->GetVelocity(velocityOut);
}

void cSphereObject::ApplyForce(const glm::vec3& force)
{
	this->_component->ApplyForce(force);
}

int cSphereObject::GetMassType()
{
	EnterCriticalSection(&object_lock);
	int result = _physicsType;
	LeaveCriticalSection(&object_lock);
	return result;
}

void cSphereObject::SetMassType(int physicsType)
{
	EnterCriticalSection(&object_lock);
	_physicsType = physicsType;
	LeaveCriticalSection(&object_lock);
}

std::string cSphereObject::GetPlaneType()
{
	EnterCriticalSection(&object_lock);
	std::string result = this->_planeType;
	LeaveCriticalSection(&object_lock);
	return result;
}

void cSphereObject::SetPlaneType(std::string planeType)
{
	EnterCriticalSection(&object_lock);
	this->_planeType = planeType;
	LeaveCriticalSection(&object_lock);
}

void cSphereObject::SetComponent(nPhysics::iPhysicsComponent* component)
{
	EnterCriticalSection(&object_lock);
	this->_component = component;
	LeaveCriticalSection(&object_lock);
}

nPhysics::iPhysicsComponent* cSphereObject::GetComponent()
{
	EnterCriticalSection(&object_lock);
	nPhysics::iPhysicsComponent* result = this->_component;
	LeaveCriticalSection(&object_lock);
	return result;
}

cSphereObject::cSphereObject(nPhysics::eComponentType componentType)
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
	this->_m_uniqueID = cSphereObject::next_uniqueID;
	// Then increment the static variable
	cSphereObject::next_uniqueID++;

	this->_disableDepthBufferTest = false;
	this->_disableDepthBufferWrite = false;

	InitializeCriticalSection(&object_lock);

	return;
}

cSphereObject::~cSphereObject()
{
	DeleteCriticalSection(&object_lock);
}

void cSphereObject::MoveInRelativeDirection(glm::vec3 relativeDirection)
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

void cSphereObject::setTexture(std::string tex, int index)
{
	EnterCriticalSection(&object_lock);
	this->textures[index] = tex;
	LeaveCriticalSection(&object_lock);
}

void cSphereObject::setTextureRatio(float texRatio, int index)
{
	EnterCriticalSection(&object_lock);
	this->textureRatio[index] = texRatio;
	LeaveCriticalSection(&object_lock);
}

void cSphereObject::setTransprancyValue(float transprancyValue)
{
	EnterCriticalSection(&object_lock);
	this->transprancyValue = transprancyValue;
	LeaveCriticalSection(&object_lock);
}

void cSphereObject::setIsTranspanrancy(bool isTranspanrancy)
{
	EnterCriticalSection(&object_lock);
	this->isTranspanrancy = isTranspanrancy;
	LeaveCriticalSection(&object_lock);
}


unsigned int cSphereObject::getUniqueID(void)
{
	EnterCriticalSection(&object_lock);
	unsigned int result = this->_m_uniqueID;
	LeaveCriticalSection(&object_lock);
	return result;
}

// this variable is static, so common to all objects.
// When the object is created, the unique ID is set, and 
//	the next unique ID is incremented
//static 
unsigned int cSphereObject::next_uniqueID = 1000;	// Starting at 1000, just because
