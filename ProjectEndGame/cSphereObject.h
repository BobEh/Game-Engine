#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>
#include "iObject.h"
#include "cShooter.h"
#include <vector>

#define NUMBEROFTEXTURES 4

class cSphereObject : public iObject
{
private:
	CRITICAL_SECTION object_lock;
public:
	inline const nPhysics::eComponentType& GetComponentType() { return mComponentType; }

	virtual void GetTransform(glm::mat4& transformOut);

	virtual void GetPosition(glm::vec3& positionOut);
	virtual void SetPosition(glm::vec3 positionIn);

	//virtual void ApplyForce(const glm::vec3& force);

	virtual void GetVelocity(glm::vec3& velocityOut);

	virtual void ApplyForce(const glm::vec3& force);

	virtual int GetMassType();
	virtual void SetMassType(int physicsType);

	virtual std::string GetPlaneType();
	virtual void SetPlaneType(std::string planeType);

	virtual void SetComponent(nPhysics::iPhysicsComponent* component);
	virtual nPhysics::iPhysicsComponent* GetComponent();

	nPhysics::eComponentType mComponentType;

	cSphereObject(nPhysics::eComponentType componentType);
	~cSphereObject();
	cSphereObject() = delete;

	unsigned int getUniqueID(void);

	cShooter* pShooter;

	void Shoot();

	nPhysics::iPhysicsComponent* _component;

	virtual cSimpleAssimpSkinnedMesh* getSM();

	virtual std::string getAnimation();
	virtual std::string getBehaviour();
	std::string getMeshName();
	unsigned int getFriendlyIDNumber();
	std::string getFriendlyName();
	glm::vec3 getPositionXYZ();
	glm::quat getRotationXYZ();
	float getScale();
	glm::mat4 getMatWorld();
	glm::vec4 getObjectColourRGBA();
	glm::vec4 getDiffuseColour();
	glm::vec4 getSpecularColour();
	glm::vec3 getVelocity();
	float getVelocityX();
	float getVelocityY();
	float getVelocityZ();
	glm::vec3 getAccel();
	float getInverseMass();
	std::string getPhysicsShapeType();
	bool getIsWireframe();
	glm::vec4 getDebugColour();
	float get_SPHERE_radius();
	bool getIsVisible();
	bool getDisableDepthBufferTest();
	bool getDisableDepthBufferWrite();
	std::vector<glm::vec3> getTestPoints();

	std::string getTextures(int index);
	float getTextureRatio(int index);

	float getTransprancyValue();
	bool getIsTranspanrancy();
	//setters
	virtual void setAnimation(std::string animation);
	virtual void setBehaviour(std::string behaviour);
	virtual void setSM(cSimpleAssimpSkinnedMesh* skinnedMesh);
	void setMeshName(std::string name);
	void setFriendlyIDNumber(unsigned int IDNumber);
	void setFriendlyName(std::string friendlyName);
	void setPositionXYZ(glm::vec3 positionXYZ);
	void setRotationXYZ(glm::quat rotationXYZ);
	void setScale(float scale);
	void setMatWorld(glm::mat4 matWorld);
	void setObjectColourRGBA(glm::vec4 colourRGBA);
	void setDiffuseColour(glm::vec4 diffuseColourRGBA);
	void setSpecularColour(glm::vec4 specularColourRGBA);
	void setVelocity(glm::vec3 velocityXYZ);
	void setVelocityX(float velocityX);
	void setVelocityY(float velocityY);
	void setVelocityZ(float velocityZ);
	void setAccel(glm::vec3 accelXYZ);
	void setInverseMass(float inverseMass);
	void setPhysicsShapeType(std::string physicsShapeType);
	void setIsWireframe(bool isWireFrame);
	void setDebugColour(glm::vec4 debugColourRGBA);
	void set_SPHERE_radius(float radius);
	void setIsVisible(bool isVisible);
	void setDisableDepthBufferTest(bool disableDepthBufferTest);
	void setDisableDepthBufferWrite(bool disableDwpthBufferWrite);
	void addTestPoint(glm::vec3);

	void MoveInRelativeDirection(glm::vec3 relativeDirection);

	void setTexture(std::string tex, int index);
	void setTextureRatio(float texRatio, int index);

	void setTransprancyValue(float transprancyValue);
	void setIsTranspanrancy(bool isTranspanrancy);


private:
	std::string _Animation;
	std::string _Behaviour;
	cSimpleAssimpSkinnedMesh* pSM;
	int _physicsType;
	std::string _planeType;
	glm::quat qRotation;
	std::vector<glm::vec3> testPoints;
	static unsigned int next_uniqueID;
	unsigned int _m_uniqueID;
	std::string _meshName;
	unsigned int _friendlyIDNumber;
	std::string _friendlyName;
	glm::vec3  _positionXYZ;
	float _scale;
	glm::mat4 _matWorld;
	glm::vec4  _objectColourRGBA;
	glm::vec4 _diffuseColour;
	glm::vec4 _specularColour;
	glm::vec3 _velocity;
	float _velocityX;
	float _velocityY;
	float _velocityZ;
	glm::vec3 _accel;
	float _HACK_speed;
	float _HACK_AngleAroundYAxis;
	float _inverseMass;
	std::string _physicsShapeType;
	bool _isWireframe;
	glm::vec4 _debugColour;
	float _SPHERE_radius;
	bool _isVisible;
	bool _disableDepthBufferTest;
	bool _disableDepthBufferWrite;

	std::string textures[NUMBEROFTEXTURES];
	float textureRatio[NUMBEROFTEXTURES];

	float transprancyValue;
	bool isTranspanrancy;
};