#pragma once
#include "includes.h"
#include "globals.h"

void SetDeltaTime()
{
	double currentTime = glfwGetTime();

	// Frame time... (how many seconds since last frame)
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	const double SOME_HUGE_TIME = 0.1;	// 100 ms;
	if (deltaTime > SOME_HUGE_TIME)
	{
		deltaTime = SOME_HUGE_TIME;
	}

	avgDeltaTimeThingy.addValue(deltaTime);
}

void PlaceLights()
{
	int lightIndex = 0;
	for (lightIndex; lightIndex < pLightsVec.size(); ++lightIndex)
	{
		std::string positionString = "theLights[" + std::to_string(lightIndex) + "].position";
		std::string diffuseString = "theLights[" + std::to_string(lightIndex) + "].diffuse";
		std::string specularString = "theLights[" + std::to_string(lightIndex) + "].specular";
		std::string attenString = "theLights[" + std::to_string(lightIndex) + "].atten";
		std::string directionString = "theLights[" + std::to_string(lightIndex) + "].direction";
		std::string param1String = "theLights[" + std::to_string(lightIndex) + "].param1";
		std::string param2String = "theLights[" + std::to_string(lightIndex) + "].param2";

		GLint position = glGetUniformLocation(shaderProgID, positionString.c_str());
		GLint diffuse = glGetUniformLocation(shaderProgID, diffuseString.c_str());
		GLint specular = glGetUniformLocation(shaderProgID, specularString.c_str());
		GLint atten = glGetUniformLocation(shaderProgID, attenString.c_str());
		GLint direction = glGetUniformLocation(shaderProgID, directionString.c_str());
		GLint param1 = glGetUniformLocation(shaderProgID, param1String.c_str());
		GLint param2 = glGetUniformLocation(shaderProgID, param2String.c_str());

		glUniform4f(position, pLightsVec.at(lightIndex)->getPositionX(), pLightsVec.at(lightIndex)->getPositionY(), pLightsVec.at(lightIndex)->getPositionZ(), 1.0f);
		glUniform4f(diffuse, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(specular, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(atten, pLightsVec.at(lightIndex)->getConstAtten(),  /* constant attenuation */	pLightsVec.at(lightIndex)->getLinearAtten(),  /* Linear */ pLightsVec.at(lightIndex)->getQuadraticAtten(),	/* Quadratic */  1000000.0f);	// Distance cut off

		glUniform4f(param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f);
		glUniform4f(param2, 1.0f /*Light is on*/, 0.0f, 0.0f, 1.0f);
	}
}

int jumpCount = 0;
int rollCount = 0;
int dyingCount = 0;
int attackCount = 0;

void DrawFirstPass()
{
	glm::quat moonRotation = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(-0.04f)));
	pMoon->setRotationXYZ(pMoon->getRotationXYZ() * moonRotation);
	glm::quat marsRotation = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(0.04f)));
	pMars->setRotationXYZ(pMars->getRotationXYZ() * marsRotation);
	// **************************************************
	// **************************************************
	// Loop to draw everything in the scene

	//Draw everything to the external frame buffer
	// (I get the frame buffer ID, and use that)
	glBindFramebuffer(GL_FRAMEBUFFER, pTheFBO->ID);

	pTheFBO->clearBuffers(true, true);

	// set the passNumber to 0
	passNumber_UniLoc = glGetUniformLocation(shaderProgID, "passNumber");
	glUniform1i(passNumber_UniLoc, 0);

	PlaceLights();

	for (int index = 0; index != ::g_vec_pAIGameObjects.size(); index++)
	{

		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pAIGameObjects[index];

		DrawObject(matModel, pCurrentObject, shaderProgID, pTheVAOManager);

	}//for (int index...

	//glm::quat rotation = glm::quat(glm::vec3(0.0f, 0.1f, 0.0f));
	//pSphere->setRotationXYZ(pSphere->getRotationXYZ()* rotation);

	for (int index = 0; index != ::g_vec_pAIEnvironmentObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pAIEnvironmentObjects[index];

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	for (int index = 0; index != ::g_vec_pAIEnemyObjects.size(); index++)
	{
		glm::vec3 enemyPositionRotation = g_vec_pAIEnemyObjects.at(index)->getPositionXYZ() * glm::vec3(-1.0f);
		glm::vec3 playerPositionRotation = pMainShip->getPositionXYZ() * glm::vec3(-1.0f);
		glm::vec3 forwardRotation = glm::normalize(enemyPositionRotation - playerPositionRotation);
		glm::vec3 yRotation = glm::vec3(0.0f, (forwardRotation.x - forwardRotation.z), 0.0f);
		//g_vec_pEnemyObjects.at(index)->setRotationXYZ(yRotation);
		g_vec_pAIEnemyObjects.at(index)->setRotationXYZ(forwardRotation);
		if (g_vec_pAIEnemyObjects.at(index)->getBehaviour() == "seek")
		{
			glm::vec4 forwardDirObject = glm::vec4(0.0f, 0.0f, 30.0f, 1.0f);

			glm::mat4 matModelLooking = glm::mat4(1.0f);	// Identity

			// Roation
			// Constructor for the GLM mat4x4 can take a quaternion
			glm::mat4 matRotation = glm::mat4(pMainShip->getRotationXYZ());
			matModelLooking *= matRotation;
			// *******************

			// Like in the vertex shader, I mulitly the test points
			// by the model matrix (MUST be a VEC4 because it's a 4x4 matrix)
			glm::vec4 forwardInWorldSpace = matModelLooking * forwardDirObject;

			glm::vec3 enemyLooking = g_vec_pAIEnemyObjects.at(index)->getPositionXYZ() - pMainShip->getPositionXYZ();
			//glm::vec3 playerLooking = glm::vec3(forwardInWorldSpace);
			glm::vec3 normalEnemy = glm::normalize(enemyLooking);
			//glm::vec3 normalPlayer = glm::normalize(playerLooking);
			float angle = glm::dot(normalEnemy - glm::vec3(forwardInWorldSpace), glm::vec3(forwardInWorldSpace));
			if (angle >= 0.99f)
			{
				g_vec_pAIEnemyObjects.at(index)->setBehaviour("flee");
				Flee* fleeBehaviour = new Flee(g_vec_pAIEnemyObjects.at(index), pMainShip);
				gAIManager->SetBehaviour(g_vec_pAIEnemyObjects.at(index), fleeBehaviour);
				g_vec_pAIEnemyObjects.at(index)->setTexture("blue.bmp", 1);
			}
		}
		if (g_vec_pAIEnemyObjects.at(index)->getBehaviour() == "flee")
		{
			glm::vec3 enemyLooking = g_vec_pAIEnemyObjects.at(index)->getPositionXYZ() - pMainShip->getPositionXYZ();
			glm::vec3 playerLooking = glm::vec3(0.0f, 0.0f, 1.0f);
			glm::vec3 normalEnemy = glm::normalize(enemyLooking);
			glm::vec3 normalPlayer = glm::normalize(playerLooking);
			float angle = glm::dot(normalEnemy, normalPlayer);
			if (angle <= 0.99f/* || glm::distance(g_vec_pGameObjects.at(index)->getPositionXYZ(), pSphere->getPositionXYZ()) > 100.0f*/)
				//if (angle <= 0.8f /*|| angle >= 0.3f*/)
			{
				g_vec_pAIEnemyObjects.at(index)->setBehaviour("seek");
				Seek* seekBehaviour = new Seek(g_vec_pAIEnemyObjects.at(index), pMainShip);
				gAIManager->SetBehaviour(g_vec_pAIEnemyObjects.at(index), seekBehaviour);
				g_vec_pAIEnemyObjects.at(index)->setTexture("red.bmp", 1);
			}
		}
		if (g_vec_pAIEnemyObjects.at(index)->getBehaviour() == "pursue")
		{
			float bulletDistance = glm::distance(g_vec_pAIEnemyObjects.at(index)->getPositionXYZ(), gPlayerBullet->getPositionXYZ());
			if (bulletDistance < 30.0f)
			{
				g_vec_pAIEnemyObjects.at(index)->setBehaviour("evade");
				Evade* fleeBehaviour = new Evade(g_vec_pAIEnemyObjects.at(index), gPlayerBullet);
				gAIManager->SetBehaviour(g_vec_pAIEnemyObjects.at(index), fleeBehaviour);
				g_vec_pAIEnemyObjects.at(index)->setTexture("blue.bmp", 1);
			}
		}
		if (g_vec_pAIEnemyObjects.at(index)->getBehaviour() == "evade")
		{
			float bulletDistance = glm::distance(g_vec_pAIEnemyObjects.at(index)->getPositionXYZ(), gPlayerBullet->getPositionXYZ());
			if (bulletDistance > 30.0f)
			{
				g_vec_pAIEnemyObjects.at(index)->setBehaviour("pursue");
				Pursue* seekBehaviour = new Pursue(g_vec_pAIEnemyObjects.at(index), pMainShip);
				gAIManager->SetBehaviour(g_vec_pAIEnemyObjects.at(index), seekBehaviour);
				g_vec_pAIEnemyObjects.at(index)->setTexture("purple.bmp", 1);
			}
		}
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pAIEnemyObjects[index];

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	gAIManager->update(deltaTime);

	for (int index = 0; index != g_vec_pExplosionObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = g_vec_pExplosionObjects.at(index);

		DrawObject(matModel, pCurrentObject, shaderProgID, pTheVAOManager);

		if (g_vec_pExplosionObjects.at(index)->getVelocity().x < 0)
		{
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * glm::vec3(-1.0f, -1.0f, -1.0f));
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * glm::vec3(0.99f, 1.0f, 1.0f));
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * glm::vec3(-1.0f, -1.0f, -1.0f));
		}
		if (g_vec_pExplosionObjects.at(index)->getVelocity().y < 0)
		{
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * glm::vec3(-1.0f, -1.0f, -1.0f));
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * glm::vec3(1.0f, 0.99f, 1.0f));
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * glm::vec3(-1.0f, -1.0f, -1.0f));
		}
		if (g_vec_pExplosionObjects.at(index)->getVelocity().z < 0)
		{
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * glm::vec3(-1.0f, -1.0f, -1.0f));
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * glm::vec3(1.0f, 1.0f, 0.99f));
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * glm::vec3(-1.0f, -1.0f, -1.0f));
		}
		glm::vec3 slowX = glm::vec3(0.99f, 1.0f, 1.0f);
		glm::vec3 slowY = glm::vec3(1.0f, 0.99f, 1.0f);
		glm::vec3 slowZ = glm::vec3(1.0f, 1.0f, 0.99f);
		if (g_vec_pExplosionObjects.at(index)->getVelocity().x > 0)
		{
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * slowX);
		}
		if (g_vec_pExplosionObjects.at(index)->getVelocity().y > 0)
		{
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * slowY);
		}
		if (g_vec_pExplosionObjects.at(index)->getVelocity().z > 0)
		{
			g_vec_pExplosionObjects.at(index)->setVelocity(g_vec_pExplosionObjects.at(index)->getVelocity() * slowZ);
		}

		if (g_vec_pExplosionObjects.at(index)->getVelocity().x < 1.0f && g_vec_pExplosionObjects.at(index)->getVelocity().y < 1.0f && g_vec_pExplosionObjects.at(index)->getVelocity().z < 1.0f)
		{
			g_vec_pExplosionObjects.erase(g_vec_pExplosionObjects.begin() + index);
			break;
		}
	}

	for (int index = 0; index != ::g_vec_pAIEnemyObjects.size(); index++)
	{
		if (g_vec_pAIEnemyObjects.at(index)->getBehaviour() == "seek")
		{
			glm::vec4 forwardDirObject = glm::vec4(0.0f, 0.0f, 30.0f, 1.0f);

			glm::mat4 matModelLooking = glm::mat4(1.0f);	// Identity

			// Roation
			// Constructor for the GLM mat4x4 can take a quaternion
			glm::mat4 matRotation = glm::mat4(pMainShip->getRotationXYZ());
			matModelLooking *= matRotation;
			// *******************

			// Like in the vertex shader, I mulitly the test points
			// by the model matrix (MUST be a VEC4 because it's a 4x4 matrix)
			glm::vec4 forwardInWorldSpace = matModelLooking * forwardDirObject;

			glm::vec3 enemyLooking = g_vec_pAIEnemyObjects.at(index)->getPositionXYZ() - pMainShip->getPositionXYZ();
			//glm::vec3 playerLooking = glm::vec3(forwardInWorldSpace);
			glm::vec3 normalEnemy = glm::normalize(enemyLooking);
			//glm::vec3 normalPlayer = glm::normalize(playerLooking);
			float angle = glm::dot(normalEnemy, glm::vec3(forwardInWorldSpace));
			if (angle >= 0.98f)
			{
				g_vec_pAIEnemyObjects.at(index)->setBehaviour("flee");
				Flee* fleeBehaviour = new Flee(g_vec_pAIEnemyObjects.at(index), pMainShip);
				gAIManager->SetBehaviour(g_vec_pAIEnemyObjects.at(index), fleeBehaviour);
				g_vec_pAIEnemyObjects.at(index)->setTexture("blue.bmp", 1);
			}
		}
		if (g_vec_pAIEnemyObjects.at(index)->getBehaviour() == "flee")
		{
			glm::vec3 enemyLooking = g_vec_pAIEnemyObjects.at(index)->getPositionXYZ() - pMainShip->getPositionXYZ();
			glm::vec3 playerLooking = glm::vec3(0.0f, 0.0f, 1.0f);
			glm::vec3 normalEnemy = glm::normalize(enemyLooking);
			glm::vec3 normalPlayer = glm::normalize(playerLooking);
			float angle = glm::dot(normalEnemy, normalPlayer);
			if (angle <= 0.99f/* || glm::distance(g_vec_pGameObjects.at(index)->getPositionXYZ(), pSphere->getPositionXYZ()) > 100.0f*/)
				//if (angle <= 0.8f /*|| angle >= 0.3f*/)
			{
				g_vec_pAIEnemyObjects.at(index)->setBehaviour("seek");
				Seek* seekBehaviour = new Seek(g_vec_pAIEnemyObjects.at(index), pMainShip);
				gAIManager->SetBehaviour(g_vec_pAIEnemyObjects.at(index), seekBehaviour);
				g_vec_pAIEnemyObjects.at(index)->setTexture("red.bmp", 1);
			}
		}
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pAIEnemyObjects[index];

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	pPhsyics->IntegrationStep(g_vec_pAIGameObjects, 0.03f);
	pPhsyics->IntegrationStep(g_vec_pAIEnemyObjects, 0.03f);
	pPhsyics->IntegrationStep(g_vec_pExplosionObjects, 0.03f);

	if (pMainShip->getVelocity().x < 0)
	{
		pMainShip->setVelocity(pMainShip->getVelocity() * glm::vec3(-1.0f, -1.0f, -1.0f));
		pMainShip->setVelocity(pMainShip->getVelocity() * glm::vec3(0.95f, 1.0f, 1.0f));
		pMainShip->setVelocity(pMainShip->getVelocity() * glm::vec3(-1.0f, -1.0f, -1.0f));
	}
	if (pMainShip->getVelocity().z < 0)
	{
		pMainShip->setVelocity(pMainShip->getVelocity() * glm::vec3(-1.0f, -1.0f, -1.0f));
		pMainShip->setVelocity(pMainShip->getVelocity() * glm::vec3(1.0f, 1.0f, 0.95f));
		pMainShip->setVelocity(pMainShip->getVelocity() * glm::vec3(-1.0f, -1.0f, -1.0f));
	}
	glm::vec3 slowX = glm::vec3(0.95, 1.0f, 1.0f);
	glm::vec3 slowZ = glm::vec3(1.0f, 1.0f, 0.95);
	if (pMainShip->getVelocity().x > 0)
	{
		pMainShip->setVelocity(pMainShip->getVelocity() * slowX);
	}
	if (pMainShip->getVelocity().z > 0)
	{
		pMainShip->setVelocity(pMainShip->getVelocity() * slowZ);
	}

	for (int k = 0; k < ::g_vec_pAIEnemyObjects.size(); k++)
	{
		glm::vec3 hitPosition = gPlayerBullet->getPositionXYZ();
		//player with enemy
		float playerDistance = glm::distance(g_vec_pAIEnemyObjects.at(k)->getPositionXYZ(), pMainShip->getPositionXYZ());
		if (playerDistance < 4.2f)
		{
			if (g_vec_pAIEnemyObjects.at(k)->getFriendlyName() != "bullet")
			{
				glm::vec3 hitPosition = g_vec_pAIEnemyObjects.at(k)->getPositionXYZ();
				//g_vec_pEnemyObjects.erase(g_vec_pEnemyObjects.begin() + k);
				//pSphere->setPositionXYZ(glm::vec3(0.0f, 10.0f, 0.0f));
				for (int i = 0; i < 3; i++)
				{
					iObject* pCrashAsteroid = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
					pCrashAsteroid->setMeshName("asteroid");
					pCrashAsteroid->setFriendlyName("asteroid");	// We use to search 
					pCrashAsteroid->setPositionXYZ(g_vec_pAIEnemyObjects.at(k)->getPositionXYZ());
					pCrashAsteroid->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
					pCrashAsteroid->setScale(randInRange(0.005f, 0.02f));
					pCrashAsteroid->setTexture("moon.bmp", 1);
					pCrashAsteroid->setTextureRatio(1, 1);
					pCrashAsteroid->setTransprancyValue(1.0f);
					pCrashAsteroid->setVelocity(glm::vec3(randInRange(-10.0f, 10.0f), randInRange(-10.0f, 10.0f), randInRange(-10.0f, 10.0f)));
					pCrashAsteroid->set_SPHERE_radius(1.0f);
					pCrashAsteroid->setInverseMass(1.0f);
					pCrashAsteroid->setIsVisible(true);
					pCrashAsteroid->setIsWireframe(false);
					//nPhysics::sBallDef physicsAsteroid;
					//physicsAsteroid.Mass = 1.0f;
					//physicsAsteroid.Position = g_vec_pAIEnemyObjects.at(k)->getPositionXYZ();
					//physicsAsteroid.Radius = 1.0f;
					//float asteroidScale = randInRange(0.005f, 0.02f);
					//physicsAsteroid.Scale = glm::vec3(asteroidScale);
					//nPhysics::iBallComponent* pAsteroidPhysics = physicsFactory->CreateBall(physicsAsteroid);
					//g_vec_pGameComponentObjects.push_back(pAsteroidPhysics);
					//pCrashAsteroid->SetComponent(pAsteroidPhysics);
					g_vec_pExplosionObjects.push_back(pCrashAsteroid);
					//physicsWorld->AddComponent(pAsteroidPhysics);
				}
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			}
		}
		//bullet with enemy
		float bulletDistance = glm::distance(g_vec_pAIEnemyObjects.at(k)->getPositionXYZ(), gPlayerBullet->getPositionXYZ());
		if (bulletDistance < 4.2f)
		{
			if (g_vec_pAIEnemyObjects.at(k)->getFriendlyName() != "bullet")
			{

				for (int i = 0; i < 3; i++)
				{
					iObject* pHitAsteroid = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
					pHitAsteroid->setMeshName("asteroid");
					pHitAsteroid->setFriendlyName("asteroid");	// We use to search 
					pHitAsteroid->setPositionXYZ(g_vec_pAIEnemyObjects.at(k)->getPositionXYZ());
					pHitAsteroid->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
					pHitAsteroid->setScale(randInRange(0.005f, 0.02f));
					pHitAsteroid->setTexture("moon.bmp", 1);
					pHitAsteroid->setTextureRatio(1, 1);
					pHitAsteroid->setTransprancyValue(1.0f);
					pHitAsteroid->setVelocity(glm::vec3(randInRange(-10.0f, 10.0f), randInRange(-10.0f, 10.0f), randInRange(-10.0f, 10.0f)));
					pHitAsteroid->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
					pHitAsteroid->set_SPHERE_radius(1.0f);
					pHitAsteroid->setInverseMass(1.0f);
					pHitAsteroid->setIsVisible(true);
					pHitAsteroid->setIsWireframe(false);
					//nPhysics::sBallDef physicsAsteroid;
					//physicsAsteroid.Mass = 1.0f;
					//physicsAsteroid.Position = g_vec_pAIEnemyObjects.at(k)->getPositionXYZ();
					//physicsAsteroid.Radius = 1.0f;
					//float asteroidScale = randInRange(0.005f, 0.02f);
					//physicsAsteroid.Scale = glm::vec3(asteroidScale);
					//nPhysics::iBallComponent* pAsteroidPhysics = physicsFactory->CreateBall(physicsAsteroid);
					//g_vec_pGameComponentObjects.push_back(pAsteroidPhysics);
					//pHitAsteroid->SetComponent(pAsteroidPhysics);
					g_vec_pExplosionObjects.push_back(pHitAsteroid);
					//physicsWorld->AddComponent(pAsteroidPhysics);
				}
				g_vec_pAIEnemyObjects.erase(g_vec_pAIEnemyObjects.begin() + k);
				gPlayerBullet->setVelocity(glm::vec3(0.0f));
				gPlayerBullet->setPositionXYZ(glm::vec3(0.0f, 0.0f, -100.0f));

				break;
			}
		}
		//Fire on approach
		if (g_vec_pAIEnemyObjects.at(k)->getBehaviour() == "approach");
		{
			float approachDistance = glm::distance(g_vec_pAIEnemyObjects.at(k)->getPositionXYZ(), pMainShip->getPositionXYZ());
			int justOne = 1;
			if (approachDistance < 16.0f && justOne < 2)
			{
				//justOne++;
				//iObject* newEnemyBullet = pFactory->CreateObject("sphere");
				//newEnemyBullet->setMeshName("sphere");
				//newEnemyBullet->setFriendlyName("tempbullet");	// We use to search 
				//newEnemyBullet->setPositionXYZ(g_vec_pEnemyObjects.at(k)->getPositionXYZ());
				//newEnemyBullet->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
				//newEnemyBullet->setScale(1.0f);
				////gPlayerBullet->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				//newEnemyBullet->setDebugColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				//newEnemyBullet->setTexture("red.bmp", 1);
				//newEnemyBullet->setTextureRatio(1, 1);
				//newEnemyBullet->setTransprancyValue(1.0f);
				//newEnemyBullet->setVelocity(g_vec_pEnemyObjects.at(k)->getPositionXYZ() - pSphere->getPositionXYZ());
				//newEnemyBullet->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
				//newEnemyBullet->set_SPHERE_radius(1.0f);
				//newEnemyBullet->setInverseMass(1.0f);
				//newEnemyBullet->setIsVisible(true);
				//newEnemyBullet->setIsWireframe(false);
				//g_vec_pEnemyObjects.push_back(newEnemyBullet);
			}
		}
		if (g_vec_pAIEnemyObjects.at(k)->getFriendlyName() == "tempbullet")
		{
			float offScreenDistance = glm::distance(g_vec_pAIEnemyObjects.at(k)->getPositionXYZ(), pMainShip->getPositionXYZ());
			if (offScreenDistance > 200.0f)
			{
				g_vec_pAIEnemyObjects.erase(g_vec_pAIEnemyObjects.begin() + k);
			}
		}
		if (g_vec_pAIEnemyObjects.at(k)->getBehaviour() == "wander")
		{
			if (g_vec_pAIEnemyObjects.at(k)->getVelocity().x < 0.05f && g_vec_pAIEnemyObjects.at(k)->getVelocity().z < 0.05f)
			{
				Wander* newWander = new Wander(g_vec_pAIEnemyObjects.at(k), glm::vec3(randInRange(-60.0f, 60.0f), 10.0f, randInRange(-40.0f, 40.0f)));
				gAIManager->SetBehaviour(g_vec_pAIEnemyObjects.at(k), newWander);
			}
		}
	}// end for	


	while (g_vec_pAIEnemyObjects.size() < 7)
	{
		iObject* pEnemy = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);

		//set behaviour
		int enemyDefault = 1;
		int numberOfBehavious = 5;
		int enemyBehaviour = randInRange(enemyDefault, numberOfBehavious);
		//int enemyBehaviour = 1;
		if (enemyBehaviour >= 1)
		{
			pEnemy->setBehaviour("seek");
		}
		if (enemyBehaviour >= 2)
		{
			pEnemy->setBehaviour("pursue");
		}
		if (enemyBehaviour >= 3)
		{
			pEnemy->setBehaviour("approach");
		}
		if (enemyBehaviour >= 4)
		{
			pEnemy->setBehaviour("wander");
		}
		pEnemy->setMeshName("xWingR");
		pEnemy->setPositionXYZ(glm::vec3(randInRange(-100.0f, 100.0f), 10.0f, randInRange(100.0f, 175.0f)));
		pEnemy->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pEnemy->setScale(1.0f);
		if (pEnemy->getBehaviour() == "seek")
		{
			Seek* seekBehaviour = new Seek(pEnemy, pMainShip);
			gAIManager->SetBehaviour(pEnemy, seekBehaviour);
			pEnemy->setTexture("red.bmp", 1);
		}
		if (pEnemy->getBehaviour() == "pursue")
		{
			Pursue* seekBehaviour = new Pursue(pEnemy, pMainShip);
			gAIManager->SetBehaviour(pEnemy, seekBehaviour);
			pEnemy->setTexture("purple.bmp", 1);
		}
		if (pEnemy->getBehaviour() == "approach")
		{
			Approach* seekBehaviour = new Approach(pEnemy, pMainShip);
			gAIManager->SetBehaviour(pEnemy, seekBehaviour);
			pEnemy->setTexture("white.bmp", 1);
		}
		if (pEnemy->getBehaviour() == "wander")
		{
			Wander* seekBehaviour = new Wander(pEnemy, glm::vec3(randInRange(-60.0f, 60.0f), 10.0f, randInRange(-40.0f, 40.0f)));
			gAIManager->SetBehaviour(pEnemy, seekBehaviour);
			pEnemy->setTexture("green.bmp", 1);
		}
		pEnemy->setTextureRatio(1, 1);
		pEnemy->setTransprancyValue(1.0f);
		pEnemy->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		pEnemy->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
		pEnemy->set_SPHERE_radius(1.0f);
		pEnemy->setInverseMass(1.0f);
		pEnemy->setIsVisible(true);
		pEnemy->setIsWireframe(false);

		//nPhysics::sBallDef enemyBall;
		//enemyBall.Mass = 1.0f;
		//enemyBall.Position = glm::vec3(randInRange(-100.0f, 100.0f), 10.0f, randInRange(100.0f, 175.0f));
		//enemyBall.Radius = 4.0f;
		//enemyBall.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		//nPhysics::iBallComponent* pEnemyPhysics = physicsFactory->CreateBall(enemyBall);
		//g_vec_pGameComponentObjects.push_back(pEnemyPhysics);
		//pEnemy->SetComponent(pEnemyPhysics);

		::g_vec_pAIEnemyObjects.push_back(pEnemy);
		//physicsWorld->AddComponent(pEnemy->GetComponent());
	}
}

void DrawSecondPass()
{
	if (jumping)
	{
		jumpCount++;
		if (jumpCount >= 150)
		{
			jumping = false;
			jumpCount = 0;
			currentAnimationName = "Idle";
		}
	}

	if (rolling)
	{
		rollCount++;
		if (rollCount >= 120)
		{
			rolling = false;
			rollCount = 0;
			currentAnimationName = "Idle";
		}
	}

	if (attacking)
	{
		attackCount++;
		if (attackCount >= 120)
		{
			attacking = false;
			attackCount = 0;
			currentAnimationName = "Idle";

			for (int i = 0; i < g_vec_pGameObjects.size(); i++)
			{
				if (g_vec_pGameObjects.at(i)->getFriendlyName() == "mainCharacter")
				{
					continue;
				}
				else
				{
					g_vec_pGameObjects.at(i)->setTexture("green.bmp", 1);
				}
			}
		}
	}
	// The whole scene is now drawn (to the FBO)
	iObject* pMainCharacter = pFindObjectByFriendlyName("mainCharacter");

	if (pMainCharacter != nullptr && currentAnimationName != "Jump" && currentAnimationName != "Roll" && currentAnimationName != "Dying" && currentAnimationName != "Attack")
	{
		if (pMainCharacter->getVelocity().z > 10.0f || pMainCharacter->getVelocity().x > 10.0f || pMainCharacter->getVelocity().z < -10.0f || pMainCharacter->getVelocity().x < -10.0f)
		{
			currentAnimationName = "Run";
		}
		else if (pMainCharacter->getVelocity().z > 1.0f || pMainCharacter->getVelocity().x > 1.0f || pMainCharacter->getVelocity().z < -1.0f || pMainCharacter->getVelocity().x < -1.0f)
		{
			currentAnimationName = "Walk";
		}
		//else if (pMainCharacter->getVelocity().y > 15.0f)
		//{
		//	currentAnimationName = "Jump";
		//}
		else if (pMainCharacter->getVelocity().y < -3.0f || pMainCharacter->getVelocity().y > 3.0f)
		{
			currentAnimationName = "Fall";
		}
		else
		{
			currentAnimationName = "Idle";
		}
	}

	physicsWorld->Update(deltaTime);
	gCoordinator->update(deltaTime);
	gAIManager->update(deltaTime);

	if (bLightDebugSheresOn)
	{
		DrawDebugSpheres();
	}// if (bLightDebugSheresOn) 

// 1. Disable the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. Clear the ACTUAL screen buffer
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 3. Use the FBO colour texture as the texture on that quad.
	// set the passNumber to 1
	glUniform1i(passNumber_UniLoc, 1);

	glActiveTexture(GL_TEXTURE0 + 40);
	glBindTexture(GL_TEXTURE_2D, pTheFBO->colourTexture_0_ID);

	GLuint texSampFBO_UL = glGetUniformLocation(shaderProgID, "secondPassColourTexture");
	glUniform1i(texSampFBO_UL, 40);

	// 4. Draw a single object (a triangle or quad)
	iObject* pQuadOrIsIt = pFindObjectByFriendlyName("debug_cube");
	pQuadOrIsIt->setScale(30.0f);
	pQuadOrIsIt->setIsVisible(true);
	//glm::vec3 oldLocation = glm::vec3(::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z);
	pQuadOrIsIt->setPositionXYZ(glm::vec3(::g_pFlyCamera->getAtInWorldSpace().x, ::g_pFlyCamera->getAtInWorldSpace().y, ::g_pFlyCamera->getAtInWorldSpace().z + 300));
	//pQuadOrIsIt->setPositionXYZ(glm::vec3(::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z + 100));
	pQuadOrIsIt->setIsWireframe(false);

	// Move the camera
	// Maybe set it to orthographic, etc.
	glm::mat4 matQuad = glm::mat4(1.0f);
	DrawObject(matQuad, pQuadOrIsIt, shaderProgID, pTheVAOManager);

	// set pass number back to 0 to render the rest of the scene
	glUniform1i(passNumber_UniLoc, 0);

	for (int index = 0; index != ::g_vec_pCharacterObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pCharacterObjects[index];

		//glm::vec3 resetThePosition = glm::vec3(0.0f, 0.0f, 0.0f);
		//pCurrentObject->GetComponent()->GetPosition(resetThePosition);

		//pCurrentObject->setPositionXYZ(resetThePosition);
		glm::vec3 currentVelocity;
		pCurrentObject->GetVelocity(currentVelocity);
		if (currentVelocity.x == 0.0f && currentVelocity.y == 0.0f)
		{
			currentVelocity += 0.000001f;
		}
		glm::vec3 normalizedVelocity = glm::normalize(currentVelocity);
		normalizedVelocity.z *= -1.0f;
		glm::quat orientation = glm::quatLookAt(normalizedVelocity, glm::vec3(0.0f, 1.0f, 0.0f));
		orientation.x = 0.0f;
		orientation.y *= -1.0f;
		orientation.z = 0.0f;
		pCurrentObject->setRotationXYZ(orientation);

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	for (int index = 0; index != ::g_vec_pGameObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pGameObjects[index];

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	for (int index = 0; index != ::g_vec_pEnvironmentObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pEnvironmentObjects[index];

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	for (int index = 0; index != g_vec_pClothObjects.size(); index++)
	{
		iObject* pCurrentObject = ::g_vec_pClothObjects[index];

		size_t numNodes = pCurrentObject->GetComponent()->NumNodes();

		for (int i = 0; i < numNodes; i++)
		{
			glm::mat4 matModel = glm::mat4(1.0f);
			float scale = 1.0f;
			glm::vec3 position = glm::vec3(1.0f);
			pCurrentObject->GetComponent()->GetNodeRadius(i, scale);
			pCurrentObject->GetComponent()->GetNodePosition(i, position);
			matModel = glm::scale(matModel, glm::vec3(scale));
			matModel = glm::translate(matModel, position);
			DrawObject(matModel, pCurrentObject, shaderProgID, pTheVAOManager);
		}
	}

	glm::mat4 skyMatModel2 = glm::mat4(1.0f);

	DrawObject(skyMatModel2, pSkyBoxSphere, shaderProgID, pTheVAOManager);

	if (bLightDebugSheresOn)
	{
		DrawDebugSpheres();
	}// if (bLightDebugSheresOn) 
}