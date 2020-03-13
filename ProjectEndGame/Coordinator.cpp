#include "Coordinator.h"

Coordinator::Coordinator(void) : currentCommand("none"), currentTarget(glm::vec3(0.0f, 0.0f, 0.0f)), offsetX(-60.0f), offsetZ(-60.0f), pathSection(0)
{
	this->theCoordinator = nullptr;
	//square
	squareOffsets.push_back(glm::vec3(-60.0f, 0.0f, -60.0f));
	squareOffsets.push_back(glm::vec3(-60.0f, 0.0f, -20.0f));
	squareOffsets.push_back(glm::vec3(-60.0f, 0.0f, 20.0f));
	squareOffsets.push_back(glm::vec3(-60.0f, 0.0f, 60.0f));
	squareOffsets.push_back(glm::vec3(-20.0f, 0.0f, 60.0f));
	squareOffsets.push_back(glm::vec3(-20.0f, 0.0f, -60.0f));
	squareOffsets.push_back(glm::vec3(20.0f, 0.0f, -60.0f));
	squareOffsets.push_back(glm::vec3(20.0f, 0.0f, 60.0f));
	squareOffsets.push_back(glm::vec3(60.0f, 0.0f, -60.0f));
	squareOffsets.push_back(glm::vec3(60.0f, 0.0f, -20.0f));
	squareOffsets.push_back(glm::vec3(60.0f, 0.0f, 20.0f));
	squareOffsets.push_back(glm::vec3(60.0f, 0.0f, 60.0f));

	vOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	vOffsets.push_back(glm::vec3(-30.0f, 0.0f, -30.0f));
	vOffsets.push_back(glm::vec3(-60.0f, 0.0f, -60.0f));
	vOffsets.push_back(glm::vec3(-90.0f, 0.0f, -90.0f));
	vOffsets.push_back(glm::vec3(-120.0f, 0.0f, -120.0f));
	vOffsets.push_back(glm::vec3(-150.0f, 0.0f, -150.0f));
	vOffsets.push_back(glm::vec3(30.0f, 0.0f, -30.0f));
	vOffsets.push_back(glm::vec3(60.0f, 0.0f, -60.0f));
	vOffsets.push_back(glm::vec3(90.0f, 0.0f, -90.0f));
	vOffsets.push_back(glm::vec3(120.0f, 0.0f, -120.0f));
	vOffsets.push_back(glm::vec3(150.0f, 0.0f, -150.0f));
	vOffsets.push_back(glm::vec3(180.0f, 0.0f, -180.0f));

	circleOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	circleOffsets.push_back(glm::vec3(-30.0f, 0.0f, -30.0f));
	circleOffsets.push_back(glm::vec3(-60.0f, 0.0f, -60.0f));
	circleOffsets.push_back(glm::vec3(-90.0f, 0.0f, -90.0f));
	circleOffsets.push_back(glm::vec3(-120.0f, 0.0f, -60.0f));
	circleOffsets.push_back(glm::vec3(-150.0f, 0.0f, -30.0f));
	circleOffsets.push_back(glm::vec3(-180.0f, 0.0f, 0.0f));
	circleOffsets.push_back(glm::vec3(-30.0f, 0.0f, 30.0f));
	circleOffsets.push_back(glm::vec3(-60.0f, 0.0f, 60.0f));
	circleOffsets.push_back(glm::vec3(-90.0f, 0.0f, 90.0f));
	circleOffsets.push_back(glm::vec3(-120.0f, 0.0f, 60.0f));
	circleOffsets.push_back(glm::vec3(-150.0f, 0.0f, 30.0f));

	lineOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-30.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-60.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-90.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-120.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-150.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-180.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(30.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(60.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(90.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(120.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(150.0f, 0.0f, 0.0f));

	rowsOffsets.push_back(glm::vec3(30.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(0.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(-30.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(-60.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(60.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(90.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(0.0f, 0.0f, -30.0f));
	rowsOffsets.push_back(glm::vec3(30.0f, 0.0f, -30.0f));
	rowsOffsets.push_back(glm::vec3(-60.0f, 0.0f, -30.0f));
	rowsOffsets.push_back(glm::vec3(-30.0f, 0.0f, -30.0f));
	rowsOffsets.push_back(glm::vec3(60.0f, 0.0f, -30.0f));
	rowsOffsets.push_back(glm::vec3(90.0f, 0.0f, -30.0f));

	currentFormation = squareOffsets;
}

Coordinator::~Coordinator(void)
{
}

void Coordinator::SetTarget(glm::vec3 target)
{
	this->currentTarget = target;
}

void Coordinator::Seek()
{
	/*calculates the desired velocity */
		/*Seek uses target position - current position*/
		/*Flee uses current position - target position*/
	glm::vec3 desiredVelocity = this->currentTarget - theCoordinator->getPositionXYZ();

	/* get the distance from target */
	float dist = glm::distance(theCoordinator->getPositionXYZ(), this->currentTarget);

	glm::normalize(desiredVelocity);

	float maxVelocity = 10.0f;
	float arrived = 2.0f;

	/*is the game object within the radius around the target */
	if (dist < arrived)
	{
		/* game object is approaching the target and slows down*/
		//desiredVelocity = desiredVelocity * maxVelocity * (dist / slowingRadius);
		this->currentCommand = "none";
	}
	else
	{
		/* target is far away from game object*/
		desiredVelocity *= maxVelocity;
	}

	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - theCoordinator->getVelocity();

	/* add steering force to current velocity*/
	theCoordinator->setVelocity(steer * 0.03f);

	if (glm::length(theCoordinator->getVelocity()) > maxVelocity)
	{
		theCoordinator->setVelocity(glm::normalize(theCoordinator->getVelocity()) * maxVelocity);
	}
}

void Coordinator::MakeFormation()
{
	for (int i = 0; i < vehicles.size(); i++)
	{
		/*calculates the desired velocity */
		/*Seek uses target position - current position*/
		/*Flee uses current position - target position*/
		glm::vec3 desiredPosition = glm::vec3(theCoordinator->getPositionXYZ().x, 13.0f, theCoordinator->getPositionXYZ().z) + currentFormation.at(i);

		glm::vec3 desiredVelocity = desiredPosition - vehicles.at(i)->getPositionXYZ();

		/* get the distance from target */
		float dist = glm::distance(vehicles.at(i)->getPositionXYZ(), desiredPosition);

		glm::normalize(desiredVelocity);

		//float dotAngle = glm::dot(desiredVelocity, glm::vec3(0.0f, 0.0f, 1.0f));

		//vehicles.at(i)->setRotationXYZ(glm::vec3(0.0f, dotAngle, 0.0f));

		float maxVelocity = 25.0f;
		float slowingRadius = 5.0f;

		///*is the game object within the radius around the target */
		//if (dist < slowingRadius)
		//{
		//	/* game object is approaching the target and slows down*/
		//	desiredVelocity = desiredVelocity * maxVelocity * (dist / slowingRadius);
		//}
		//else
		//{
			/* target is far away from game object*/
		desiredVelocity *= maxVelocity;
		//}

		/*calculate the steering force */
		glm::vec3 steer = desiredVelocity - vehicles.at(i)->getVelocity();

		/* add steering force to current velocity*/
		vehicles.at(i)->setVelocity(steer * 0.03f);

		if (glm::length(vehicles.at(i)->getVelocity()) > maxVelocity)
		{
			vehicles.at(i)->setVelocity(glm::normalize(vehicles.at(i)->getVelocity()) * maxVelocity);
		}
	}
}

void Coordinator::FollowPath()
{
	/*calculates the desired velocity */
	/*Seek uses target position - current position*/
	/*Flee uses current position - target position*/
	glm::vec3 desiredVelocity = this->forwardPath.at(pathSection) - theCoordinator->getPositionXYZ();

	/* get the distance from target */
	float dist = glm::distance(theCoordinator->getPositionXYZ(), this->forwardPath.at(pathSection));

	glm::normalize(desiredVelocity);

	float maxVelocity = 10.0f;
	float arrived = 2.0f;

	/*is the game object within the radius around the target */
	if (dist < arrived)
	{
		/* game object is approaching the target and slows down*/
		//desiredVelocity = desiredVelocity * maxVelocity * (dist / slowingRadius);
		//this->currentCommand = "none";
		if (pathSection < forwardPath.size() - 1)
		{
			pathSection++;
		}
		else
		{
			pathSection = 0;
			currentCommand = "none";
		}
	}
	else
	{
		/* target is far away from game object*/
		desiredVelocity *= maxVelocity;
	}

	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - theCoordinator->getVelocity();

	/* add steering force to current velocity*/
	theCoordinator->setVelocity(steer * 0.03f);

	if (glm::length(theCoordinator->getVelocity()) > maxVelocity)
	{
		theCoordinator->setVelocity(glm::normalize(theCoordinator->getVelocity()) * maxVelocity);
	}
}

float separationRadius = 50.0f;
float maxVelocity = 15.0f;
float maxForce = 15.0f;

glm::vec3 Coordinator::Separation(iObject* currentObject)
{
	glm::vec3 totalFlee = glm::vec3(0.0f);
	int neighbourCount = 0;
	for (int i = 0; i < vehicles.size(); i++)
	{
		if (currentObject->getFriendlyName() == vehicles.at(i)->getFriendlyName())
		{
			continue;
		}
		float closeEnough = glm::distance(currentObject->getPositionXYZ(), vehicles.at(i)->getPositionXYZ());
		if (closeEnough > 0.0f && closeEnough < separationRadius)
		{
			glm::vec3 fleeVector = currentObject->getPositionXYZ() - vehicles.at(i)->getPositionXYZ();
			glm::vec3 normalFlee = glm::normalize(fleeVector);
			totalFlee += normalFlee;
			neighbourCount++;
		}
	}
	glm::vec3 steerForce = glm::vec3(0.0f, 0.0f, 0.0f);
	if (neighbourCount > 0)
	{
		glm::vec3 desiredVelocity = totalFlee / (float)neighbourCount;
		glm::vec3 normalDesire = glm::normalize(desiredVelocity);
		normalDesire *= maxVelocity;
		steerForce = normalDesire - currentObject->getVelocity();
		steerForce += maxForce;
	}
	return steerForce;
}

glm::vec3 Coordinator::Alignment(iObject* currentObject)
{
	glm::vec3 totalVelocity = glm::vec3(0.0f);
	float alignmentRadius = 50.0f;
	int neighbourCount = 0;
	for (int i = 0; i < vehicles.size(); i++)
	{
		if (currentObject->getFriendlyName() == vehicles.at(i)->getFriendlyName())
		{
			continue;
		}
		float distance = glm::distance(currentObject->getPositionXYZ(), vehicles.at(i)->getPositionXYZ());
		if (distance > 0.0f && distance < alignmentRadius)
		{
			totalVelocity += vehicles.at(i)->getVelocity();
			neighbourCount++;
		}
	}
	glm::vec3 steerForce = glm::vec3(0.0f);
	if (neighbourCount > 0)
	{
		glm::vec3 desiredVelocity = totalVelocity / (float)neighbourCount;
		glm::vec3 normalDesire = glm::normalize(desiredVelocity);
		normalDesire *= maxVelocity;
		steerForce = normalDesire - currentObject->getVelocity();
		steerForce += maxForce;
	}
	return steerForce;
}

glm::vec3 Coordinator::CoSeek(iObject* currentObject, glm::vec3 target)
{
	/*calculates the desired velocity */
	/*Seek uses target position - current position*/
	/*Flee uses current position - target position*/
	glm::vec3 desiredVelocity = target - currentObject->getPositionXYZ();

	/* get the distance from target */
	float dist = glm::distance(currentObject->getPositionXYZ(), target);

	glm::normalize(desiredVelocity);

	float maxVelocity = 10.0f;

	/* target is far away from game object*/
	desiredVelocity *= maxVelocity;

	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - currentObject->getVelocity();

	return steer * 0.03f;
}

glm::vec3 Coordinator::Cohesion(iObject* currentObject)
{
	glm::vec3 totalPosition = glm::vec3(0.0f);
	float cohesionRadius = 50.0f;
	int neighbourCount = 0;
	for (int i = 0; i < vehicles.size(); i++)
	{
		if (currentObject->getFriendlyName() == vehicles.at(i)->getFriendlyName())
		{
			continue;
		}
		float distance = glm::distance(currentObject->getPositionXYZ(), vehicles.at(i)->getPositionXYZ());
		if (distance > 0.0f && distance < cohesionRadius)
		{
			totalPosition += vehicles.at(i)->getPositionXYZ();
			neighbourCount++;
		}
	}
	glm::vec3 steerForce = glm::vec3(0.0f);

	if (neighbourCount > 0)
	{
		glm::vec3 target = totalPosition / (float)neighbourCount;
		steerForce = CoSeek(currentObject, target);
	}
	return steerForce;
}

void Coordinator::Flock()
{
	for (int i = 0; i < vehicles.size(); i++)
	{
		glm::vec3 separation = Separation(vehicles.at(i));
		glm::vec3 alignment = Alignment(vehicles.at(i));
		glm::vec3 cohesion = Cohesion(vehicles.at(i));

		separation = glm::vec3(separation.x, 0.0f, separation.z);
		alignment = glm::vec3(alignment.x, 0.0f, alignment.z);
		cohesion = glm::vec3(cohesion.x, 0.0f, cohesion.z);

		separation *= 0.25f;
		alignment *= 0.13f;
		cohesion *= 0.5f;

		glm::vec3 newVelocity = vehicles.at(i)->getVelocity() + (separation + alignment + cohesion);

		vehicles.at(i)->setVelocity(newVelocity);
	}
}

void Coordinator::AddVehicle(iObject* vehicle)
{
	this->vehicles.push_back(vehicle);

}

void Coordinator::RemoveVehicle(iObject* vehicle)
{
	for (int i = 0; i < this->vehicles.size(); i++)
	{
		if (this->vehicles.at(i)->getFriendlyName() == vehicle->getFriendlyName())
		{
			this->vehicles.erase(this->vehicles.begin() + i);
		}
	}
}

void Coordinator::SetCoordinatorObject(iObject* coordinator)
{
	this->theCoordinator = coordinator;
}

iObject* Coordinator::GetCoordinatorObject()
{
	return theCoordinator;
}

void Coordinator::SetCommand(std::string command)
{
	this->currentCommand = command;
}

void Coordinator::SetFormation(std::string formation)
{
	if (formation == "square")
	{
		currentFormation = squareOffsets;
	}
	if (formation == "v")
	{
		currentFormation = vOffsets;
	}
	if (formation == "circle")
	{
		currentFormation = circleOffsets;
	}
	if (formation == "line")
	{
		currentFormation = lineOffsets;
	}
	if (formation == "rows")
	{
		currentFormation = rowsOffsets;
	}
}

template <class T>
T randInRange(T min, T max)
{
	double value =
		min + static_cast <double> (rand())
		/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
	return static_cast<T>(value);
};

void Coordinator::CreatePath()
{
	if (forwardPath.size() > 0)
	{
		forwardPath.clear();
	}
	for (int i = 0; i < 6; i++)
	{
		glm::vec3 point = glm::vec3(randInRange(-200, 200), 0.0f, randInRange(-200.0f, 200.0f));
		forwardPath.push_back(point);
	}
}

void Coordinator::ReversePath()
{
	std::reverse(forwardPath.begin(), forwardPath.end());

	if (pathSection == 0)
	{
		pathSection = forwardPath.size() - 1;
	}
	else
	{
		pathSection = forwardPath.size() - pathSection;
	}
}

glm::vec3 Coordinator::GetCurrentPathPoint()
{
	return forwardPath.at(pathSection);
}

std::vector<glm::vec3> Coordinator::GetCurrentPath()
{
	return forwardPath;
}

int Coordinator::GetCurrentPathSection()
{
	return pathSection;
}

std::string Coordinator::GetCurrentCommand()
{
	return currentCommand;
}

void Coordinator::update(float dt)
{
	if (this->currentCommand == "seek")
	{
		Seek();
	}
	if (this->currentCommand == "followPath")
	{
		FollowPath();
	}
	if (this->currentCommand == "flock")
	{
		Flock();
	}
	if (this->currentCommand != "flock")
	{
		MakeFormation();
	}
}
