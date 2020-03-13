#pragma once

#include "iObject.h"
#include "Behaviour.h"

class Coordinator {
public:
	Coordinator(void);
	~Coordinator(void);

	void SetTarget(glm::vec3 target);

	void AddVehicle(iObject* vehicle);
	void RemoveVehicle(iObject* vehicle);

	void SetCoordinatorObject(iObject* coordinator);
	iObject* GetCoordinatorObject();

	void SetCommand(std::string command);
	void SetFormation(std::string formation);
	void CreatePath();
	void ReversePath();

	glm::vec3 GetCurrentPathPoint();
	std::vector<glm::vec3> GetCurrentPath();
	int GetCurrentPathSection();
	std::string GetCurrentCommand();

	void update(float dt);

private:
	void Seek();
	void MakeFormation();
	void FollowPath();
	void Flock();
	glm::vec3 Separation(iObject* currentObject);
	glm::vec3 Alignment(iObject* currentObject);
	glm::vec3 Cohesion(iObject* currentObject);
	glm::vec3 CoSeek(iObject* currentObject, glm::vec3 target);

	std::vector<iObject*> vehicles;
	std::vector<glm::vec3> currentFormation;
	std::vector<glm::vec3> squareOffsets;
	std::vector<glm::vec3> circleOffsets;
	std::vector<glm::vec3> vOffsets;
	std::vector<glm::vec3> lineOffsets;
	std::vector<glm::vec3> rowsOffsets;
	std::vector<glm::vec3> forwardPath;
	iObject* theCoordinator;
	std::string currentCommand;
	glm::vec3 currentTarget;

	int pathSection;

	float offsetX;
	float offsetZ;
};