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

void DrawAIFBO()
{
	drawSpace = true;
	// **************************************************
	// **************************************************
	// Loop to draw everything in the scene

	//Draw everything to the external frame buffer
	// (I get the frame buffer ID, and use that)
	glBindFramebuffer(GL_FRAMEBUFFER, pAIFBO->ID);

	pAIFBO->clearBuffers(true, true);

	// set the passNumber to 0
	passNumber_UniLoc = glGetUniformLocation(shaderProgID, "passNumber");
	glUniform1i(passNumber_UniLoc, 0);

	if (currentRender != renderTag::AI)
	{
		drawSpace = true;
		for (int index = 0; index != ::g_vec_pAIGameObjects.size(); index++)
		{

			glm::mat4 matModel = glm::mat4(1.0f);

			iObject* pCurrentObject = ::g_vec_pAIGameObjects[index];
			pCurrentObject->setIsWireframe(false);

			DrawObject(matModel, pCurrentObject, shaderProgID, pTheVAOManager);

		}//for (int index...

		for (int index = 0; index != ::g_vec_pAIEnvironmentObjects.size(); index++)
		{
			glm::mat4 matModel = glm::mat4(1.0f);

			iObject* pCurrentObject = ::g_vec_pAIEnvironmentObjects[index];

			//pCurrentObject->setIsWireframe(true);
			DrawObject(matModel, pCurrentObject,
				shaderProgID, pTheVAOManager);

		}//for (int index...

		for (int index = 0; index != ::g_vec_pAIEnemyObjects.size(); index++)
		{
			glm::mat4 matModel = glm::mat4(1.0f);

			iObject* pCurrentObject = ::g_vec_pAIEnemyObjects[index];

			//pCurrentObject->setIsWireframe(true);

			DrawObject(matModel, pCurrentObject,
				shaderProgID, pTheVAOManager);

		}//for (int index...
	}
	if (currentRender == renderTag::AI)
	{
		drawSpace = false;
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
	}
}

void DrawAI()
{
	drawSpace = true;
	glm::quat moonRotation = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(-0.04f)));
	if (pMoon)
	{
		pMoon->setRotationXYZ(pMoon->getRotationXYZ() * moonRotation);
	}
	glm::quat marsRotation = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(0.04f)));
	if (pMars)
	{
		pMars->setRotationXYZ(pMars->getRotationXYZ() * marsRotation);
	}
	// **************************************************
	// **************************************************
	// Loop to draw everything in the scene

	// 1. Disable the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. Clear the ACTUAL screen buffer
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 3. Use the FBO colour texture as the texture on that quad.
	// set the passNumber to 1
	glUniform1i(passNumber_UniLoc, 1);

	glActiveTexture(GL_TEXTURE0 + 40);
	glBindTexture(GL_TEXTURE_2D, pAIFBO->colourTexture_0_ID);

	GLuint texSampFBO_UL = glGetUniformLocation(shaderProgID, "AIPassColourTexture");
	glUniform1i(texSampFBO_UL, 40);

	// 4. Draw a single object (a triangle or quad)
	iObject* pQuadOrIsIt = pFindObjectByFriendlyName("debug_cube");
	if (pQuadOrIsIt)
	{
		pQuadOrIsIt->setScale(3.0f);
		pQuadOrIsIt->setIsVisible(true);
		//glm::vec3 oldLocation = glm::vec3(::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z);
		pQuadOrIsIt->setPositionXYZ(glm::vec3(::g_pFlyCamera->getAtInWorldSpace().x, ::g_pFlyCamera->getAtInWorldSpace().y, ::g_pFlyCamera->getAtInWorldSpace().z + 300));
		//pQuadOrIsIt->setPositionXYZ(glm::vec3(::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z + 100));
		pQuadOrIsIt->setIsWireframe(false);

		// Move the camera
		// Maybe set it to orthographic, etc.
		glm::mat4 matQuad = glm::mat4(1.0f);
		//DrawObject(matQuad, pQuadOrIsIt, shaderProgID, pTheVAOManager);
	}

	// set pass number back to 0 to render the rest of the scene
	glUniform1i(passNumber_UniLoc, 0);

	for (int index = 0; index != ::g_vec_pAIGameObjects.size(); index++)
	{

		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pAIGameObjects[index];
		//pCurrentObject->setIsWireframe(true);

		DrawObject(matModel, pCurrentObject, shaderProgID, pTheVAOManager);

	}//for (int index...

	for (int index = 0; index != ::g_vec_pAIEnvironmentObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pAIEnvironmentObjects[index];

		//pCurrentObject->setIsWireframe(true);
		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	for (int index = 0; index != ::g_vec_pAIEnemyObjects.size(); index++)
	{
		glm::vec3 enemyPositionRotation = g_vec_pAIEnemyObjects.at(index)->getPositionXYZ() * glm::vec3(-1.0f);
		glm::vec3 playerPositionRotation = glm::vec3(0.0f, 0.0f, 0.0f);
		if (pMainShip)
		{
			playerPositionRotation = pMainShip->getPositionXYZ() * glm::vec3(-1.0f);
		}
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
			glm::mat4 matRotation = glm::mat4(1.0f);
			if (pMainShip)
			{
				glm::mat4 matRotation = glm::mat4(pMainShip->getRotationXYZ());
			}
			matModelLooking *= matRotation;
			// *******************

			// Like in the vertex shader, I mulitly the test points
			// by the model matrix (MUST be a VEC4 because it's a 4x4 matrix)
			glm::vec4 forwardInWorldSpace = matModelLooking * forwardDirObject;

			glm::vec3 enemyLooking = glm::vec3(0.0f);

			if (pMainShip)
			{
				glm::vec3 enemyLooking = g_vec_pAIEnemyObjects.at(index)->getPositionXYZ() - pMainShip->getPositionXYZ();
			}
			//glm::vec3 playerLooking = glm::vec3(forwardInWorldSpace);
			glm::vec3 normalEnemy = glm::normalize(enemyLooking);
			//glm::vec3 normalPlayer = glm::normalize(playerLooking);
			float angle = glm::dot(normalEnemy - glm::vec3(forwardInWorldSpace), glm::vec3(forwardInWorldSpace));
			if (angle >= 0.99f)
			{
				g_vec_pAIEnemyObjects.at(index)->setBehaviour("flee");
				if (pMainShip)
				{
					Flee* fleeBehaviour = new Flee(g_vec_pAIEnemyObjects.at(index), pMainShip);
					gAIManager->SetBehaviour(g_vec_pAIEnemyObjects.at(index), fleeBehaviour);
				}				
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

		//pCurrentObject->setIsWireframe(true);

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	gAIManager->update(deltaTime);

	for (int index = 0; index != g_vec_pExplosionObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = g_vec_pExplosionObjects.at(index);

		//pCurrentObject->setIsWireframe(true);

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
			glm::mat4 matRotation = glm::mat4(1.0f);
			if (pMainShip)
			{
				matRotation = glm::mat4(pMainShip->getRotationXYZ());

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
		}
		if (g_vec_pAIEnemyObjects.at(index)->getBehaviour() == "flee")
		{
			if (pMainShip)
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
		}
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pAIEnemyObjects[index];

		//pCurrentObject->setIsWireframe(true);

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	pAIPhsyics->IntegrationStep(g_vec_pAIGameObjects, 0.03f);
	pAIPhsyics->IntegrationStep(g_vec_pAIEnemyObjects, 0.03f);
	pAIPhsyics->IntegrationStep(g_vec_pExplosionObjects, 0.03f);

	if (pMainShip)
	{
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
	}

	for (int k = 0; k < ::g_vec_pAIEnemyObjects.size(); k++)
	{
		if (gPlayerBullet && pMainShip)
		{
			glm::vec3 hitPosition = gPlayerBullet->getPositionXYZ();
			//player with enemy
			float playerDistance = glm::distance(g_vec_pAIEnemyObjects.at(k)->getPositionXYZ(), pMainShip->getPositionXYZ());
			if (playerDistance < 4.2f)
			{
				if (g_vec_pAIEnemyObjects.at(k)->getFriendlyName() != "bullet")
				{
					glm::vec3 hitPosition = g_vec_pAIEnemyObjects.at(k)->getPositionXYZ();
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
						g_vec_pExplosionObjects.push_back(pCrashAsteroid);
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
						g_vec_pExplosionObjects.push_back(pHitAsteroid);
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

void DrawPlatformFBO()
{
	drawSpace = false;
	// **************************************************
// **************************************************
// Loop to draw everything in the scene

//Draw everything to the external frame buffer
// (I get the frame buffer ID, and use that)
	glBindFramebuffer(GL_FRAMEBUFFER, pPlatformFBO->ID);

	pPlatformFBO->clearBuffers(true, true);

	// set the passNumber to 0
	passNumber_UniLoc = glGetUniformLocation(shaderProgID, "passNumber");
	glUniform1i(passNumber_UniLoc, 0);

	if (currentRender != renderTag::Platform)
	{
		for (int index = 0; index != ::g_vec_pPlatformGameObjects.size(); index++)
		{

			glm::mat4 matModel = glm::mat4(1.0f);

			iObject* pCurrentObject = ::g_vec_pPlatformGameObjects[index];
			pCurrentObject->setIsWireframe(true);

			DrawObject(matModel, pCurrentObject, shaderProgID, pTheVAOManager);

		}//for (int index...

		for (int index = 0; index != ::g_vec_pPlatformEnvironmentObjects.size(); index++)
		{
			glm::mat4 matModel = glm::mat4(1.0f);

			iObject* pCurrentObject = ::g_vec_pPlatformEnvironmentObjects[index];

			//pCurrentObject->setIsWireframe(true);
			DrawObject(matModel, pCurrentObject,
				shaderProgID, pTheVAOManager);

		}//for (int index...

		for (int index = 0; index != ::g_vec_pPlatformEnemyObjects.size(); index++)
		{
			glm::mat4 matModel = glm::mat4(1.0f);

			iObject* pCurrentObject = ::g_vec_pPlatformEnemyObjects[index];

			//pCurrentObject->setIsWireframe(true);

			DrawObject(matModel, pCurrentObject,
				shaderProgID, pTheVAOManager);

		}//for (int index...
	}
	if (currentRender == renderTag::Platform)
	{
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
	}
}

void DrawPlatform()
{
	drawSpace = false;
	// 1. Disable the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. Clear the ACTUAL screen buffer
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 3. Use the FBO colour texture as the texture on that quad.
	// set the passNumber to 1
	glUniform1i(passNumber_UniLoc, 1);

	glActiveTexture(GL_TEXTURE0 + 40);
	glBindTexture(GL_TEXTURE_2D, pAIFBO->colourTexture_0_ID);

	GLuint texSampFBO_UL = glGetUniformLocation(shaderProgID, "AIPassColourTexture");
	glUniform1i(texSampFBO_UL, 40);

	// 4. Draw a single object (a triangle or quad)
	iObject* pQuadOrIsIt = pFindObjectByFriendlyName("cube");
	pQuadOrIsIt->setScale(10.0f);
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

	iObject* pMainCharacter = pFindObjectByFriendlyName("platformCharacter");
	std::string currentAnimationName = pMainCharacter->getAnimation();
	for (int index = 0; index != ::g_vec_pPlatformGameObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pPlatformGameObjects[index];

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	for (int index = 0; index != ::g_vec_pPlatformEnvironmentObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pPlatformEnvironmentObjects[index];

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	for (int index = 0; index != ::g_vec_pPlatformEnemyObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pPlatformEnemyObjects[index];

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	for (int index = 0; index != ::g_vec_pPlatformCharacterObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pPlatformCharacterObjects[index];

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	for (int index = 0; index != ::g_vec_pPlatformExplosionObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pPlatformExplosionObjects[index];

		DrawObject(matModel, pCurrentObject,
			shaderProgID, pTheVAOManager);

	}//for (int index...

	if (currentAnimationName != "Jump" && currentAnimationName != "Roll" && currentAnimationName != "Dying" && currentAnimationName != "Attack")
	{
		if (pMainCharacter->getVelocity().z > 21.0f)
		{
			currentAnimationName = "Run";
		}
		else if (pMainCharacter->getVelocity().z > 5.0f)
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

	enemyCount1++;
	enemyCount2++;
	enemyCount3++;
	enemyCount4++;

	iObject* pEnemy1 = pFindObjectByFriendlyName("enemy1");
	iObject* pEnemy2 = pFindObjectByFriendlyName("enemy2");
	iObject* pEnemy3 = pFindObjectByFriendlyName("enemy3");
	iObject* pEnemy4 = pFindObjectByFriendlyName("enemy4");

	if (enemyCount1 > 400)
	{
		if (pEnemy1->getRotationXYZ().y < 0)
		{
			pEnemy1->setRotationXYZ(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
		}
		else
		{
			pEnemy1->setRotationXYZ(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
		}
		enemyCount1 = 0;
	}
	if (enemyCount2 > 500)
	{
		if (pEnemy2->getRotationXYZ().y < 0)
		{
			pEnemy2->setRotationXYZ(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
		}
		else
		{
			pEnemy2->setRotationXYZ(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
		}
		enemyCount2 = 0;
	}
	if (enemyCount3 > 470)
	{
		if (pEnemy3->getRotationXYZ().y < 0)
		{
			pEnemy3->setRotationXYZ(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
		}
		else
		{
			pEnemy3->setRotationXYZ(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
		}
		enemyCount3 = 0;
	}
	if (enemyCount4 > 450)
	{
		if (pEnemy4->getRotationXYZ().y < 0)
		{
			pEnemy4->setRotationXYZ(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
		}
		else
		{
			pEnemy4->setRotationXYZ(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
		}
		enemyCount4 = 0;
	}

	pPlatformPhysics->IntegrationStep(g_vec_pPlatformEnemyObjects, 0.03f);
	pPlatformPhysics->IntegrationStep(g_vec_pPlatformCharacterObjects, 0.03f);
	pPlatformPhysics->IntegrationStep(g_vec_pPlatformExplosionObjects, 0.03f);
	pPlatformPhysics->IntegrationStep(g_vec_pPlatformCharacterObjects, 0.03f);

	// collisions for floors
	if (pMainCharacter->getVelocity().y < 1.0f)
	{
		for (int i = 0; i < g_vec_pPlatformEnvironmentObjects.size(); i++)
		{
			float collisionDistance = glm::distance(pMainCharacter->getPositionXYZ(), g_vec_pPlatformEnvironmentObjects.at(i)->getPositionXYZ());

			if (collisionDistance < 10.0f)
			{
				CollideWithFloor(pMainCharacter, g_vec_pPlatformEnvironmentObjects.at(i));
			}
		}
	}

	//robot collisions
	for (int i = 0; i < g_vec_pGameObjects.size(); i++)
	{
		if (g_vec_pGameObjects.at(i)->getFriendlyName() == "mainCharacter")
		{
			continue;
		}
		float collisionDistance = glm::distance(pMainCharacter->getPositionXYZ(), g_vec_pGameObjects.at(i)->getPositionXYZ());

		if (collisionDistance < 20.0f)
		{
			if (g_vec_pExplosionObjects.size() > 0)
			{
				break;
			}
			for (int j = 0; j < g_vec_pGameObjects.size(); j++)
			{
				if (g_vec_pGameObjects.at(j)->getFriendlyName() == "mainCharacter")
				{
					continue;
				}
				else
				{
					if (currentAnimationName != "Attack")
					{
						g_vec_pGameObjects.at(j)->setInverseMass(0.0f);
						currentAnimationName = "Dying";
						gotHit = true;
					}
					else
					{
						glm::vec3 pointOfImpact = g_vec_pGameObjects.at(i)->getPositionXYZ();

						iObject* pPiece1 = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
						pPiece1->setMeshName("piece1");
						pPiece1->setFriendlyName("piece1");	// We use to search 
						pPiece1->setPositionXYZ(pointOfImpact);
						pPiece1->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
						pPiece1->setScale(1.0f);
						pPiece1->setVelocity(glm::vec3(randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f)));
						pPiece1->setInverseMass(1.0f);
						pPiece1->setIsVisible(true);
						pPiece1->setIsWireframe(false);
						pPiece1->setTexture("green.bmp", 1);
						pPiece1->setTextureRatio(1, 1);
						pPiece1->setTransprancyValue(1.0f);
						::g_vec_pPlatformExplosionObjects.push_back(pPiece1);

						iObject* pPiece2 = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
						pPiece2->setMeshName("piece2");
						pPiece2->setFriendlyName("piece1");	// We use to search 
						pPiece2->setPositionXYZ(pointOfImpact);
						pPiece2->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
						pPiece2->setScale(1.0f);
						pPiece2->setVelocity(glm::vec3(randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f)));
						pPiece2->setInverseMass(1.0f);
						pPiece2->setIsVisible(true);
						pPiece2->setIsWireframe(false);
						pPiece2->setTexture("green.bmp", 1);
						pPiece2->setTextureRatio(1, 1);
						pPiece2->setTransprancyValue(1.0f);
						::g_vec_pPlatformExplosionObjects.push_back(pPiece2);

						iObject* pPiece3 = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
						pPiece3->setMeshName("piece3");
						pPiece3->setFriendlyName("piece1");	// We use to search 
						pPiece3->setPositionXYZ(pointOfImpact);
						pPiece3->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
						pPiece3->setScale(1.0f);
						pPiece3->setVelocity(glm::vec3(randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f)));
						pPiece3->setInverseMass(1.0f);
						pPiece3->setIsVisible(true);
						pPiece3->setIsWireframe(false);
						pPiece3->setTexture("green.bmp", 1);
						pPiece3->setTextureRatio(1, 1);
						pPiece3->setTransprancyValue(1.0f);
						::g_vec_pPlatformExplosionObjects.push_back(pPiece3);

						iObject* pPiece4 = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
						pPiece4->setMeshName("piece4");
						pPiece4->setFriendlyName("piece1");	// We use to search 
						pPiece4->setPositionXYZ(pointOfImpact);
						pPiece4->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
						pPiece4->setScale(1.0f);
						pPiece4->setVelocity(glm::vec3(randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f)));
						pPiece4->setInverseMass(1.0f);
						pPiece4->setIsVisible(true);
						pPiece4->setIsWireframe(false);
						pPiece4->setTexture("green.bmp", 1);
						pPiece4->setTextureRatio(1, 1);
						pPiece4->setTransprancyValue(1.0f);
						::g_vec_pPlatformExplosionObjects.push_back(pPiece4);

						iObject* pPiece5 = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
						pPiece5->setMeshName("piece5");
						pPiece5->setFriendlyName("piece1");	// We use to search 
						pPiece5->setPositionXYZ(pointOfImpact);
						pPiece5->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
						pPiece5->setScale(1.0f);
						pPiece5->setVelocity(glm::vec3(randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f)));
						pPiece5->setInverseMass(1.0f);
						pPiece5->setIsVisible(true);
						pPiece5->setIsWireframe(false);
						pPiece5->setTexture("green.bmp", 1);
						pPiece5->setTextureRatio(1, 1);
						pPiece5->setTransprancyValue(1.0f);
						::g_vec_pPlatformExplosionObjects.push_back(pPiece5);

						iObject* pPiece6 = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
						pPiece6->setMeshName("piece6");
						pPiece6->setFriendlyName("piece1");	// We use to search 
						pPiece6->setPositionXYZ(pointOfImpact);
						pPiece6->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
						pPiece6->setScale(1.0f);
						pPiece6->setVelocity(glm::vec3(randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f), randInRange(10.0f, 30.0f)));
						pPiece6->setInverseMass(1.0f);
						pPiece6->setIsVisible(true);
						pPiece6->setIsWireframe(false);
						pPiece6->setTexture("green.bmp", 1);
						pPiece6->setTextureRatio(1, 1);
						pPiece6->setTransprancyValue(1.0f);
						::g_vec_pPlatformExplosionObjects.push_back(pPiece6);

						g_vec_pPlatformEnemyObjects.at(i)->setInverseMass(0.0f);
						g_vec_pPlatformEnemyObjects.at(i)->setIsVisible(false);
						g_vec_pPlatformEnemyObjects.at(i)->setPositionXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
						//g_vec_pGameObjects.erase(g_vec_pGameObjects.begin() + j);
						break;
					}
				}
			}
		}
	}

	pMainCharacter->setAnimation(currentAnimationName);

	// collisions for walls
	if (pMainCharacter->getPositionXYZ().x > -10.0f)
	{
		pMainCharacter->setVelocity(glm::reflect(pMainCharacter->getVelocity(), glm::vec3(-1.0f, 0.0f, 0.0f)));
		// lose some energy
		glm::vec3 nComponent = glm::proj(pMainCharacter->getVelocity(), glm::vec3(0.0f, 1.0f, 0.0f));
		// rewind
		glm::vec3 setVelocity = pMainCharacter->getVelocity() - nComponent * 0.2f;
		pMainCharacter->setVelocity(setVelocity);
	}

	if (pMainCharacter->getPositionXYZ().x < -870.0f)
	{
		pMainCharacter->setVelocity(glm::reflect(pMainCharacter->getVelocity(), glm::vec3(1.0f, 0.0f, 0.0f)));
		// lose some energy
		glm::vec3 nComponent = glm::proj(pMainCharacter->getVelocity(), glm::vec3(0.0f, 1.0f, 0.0f));
		// rewind
		glm::vec3 setVelocity = pMainCharacter->getVelocity() - nComponent * 0.2f;
		pMainCharacter->setVelocity(setVelocity);
	}
}

void DrawSecondPass()
{
	iObject* pMainCharacter = pFindObjectByFriendlyName("mainCharacter");
	std::string currentAnimationName = "Idle";
	if (pMainCharacter)
	{
		currentAnimationName = pMainCharacter->getAnimation();
	}
	else 
	drawSpace = false;
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

// 1. Disable the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. Clear the ACTUAL screen buffer
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 3. Use the FBO colour texture as the texture on that quad.
	// set the passNumber to 1
	glUniform1i(passNumber_UniLoc, 1);

	glActiveTexture(GL_TEXTURE0 + 40);
	glBindTexture(GL_TEXTURE_2D, pAIFBO->colourTexture_0_ID);

	GLuint texSampAIFBO_UL = glGetUniformLocation(shaderProgID, "AIPassColourTexture");
 	glUniform1i(texSampAIFBO_UL, 40);

	// 4. Draw a single object (a triangle or quad)
	iObject* pAIQuad = pFindObjectByFriendlyName("cube");
	if (pAIQuad)
	{
		pAIQuad->setScale(5.0f);
		pAIQuad->setIsVisible(true);
		//glm::vec3 oldLocation = glm::vec3(::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z);
		pAIQuad->setPositionXYZ(glm::vec3(::g_pFlyCamera->getAtInWorldSpace().x, ::g_pFlyCamera->getAtInWorldSpace().y, ::g_pFlyCamera->getAtInWorldSpace().z + 300));
		//pQuadOrIsIt->setPositionXYZ(glm::vec3(::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z + 100));
		pAIQuad->setIsWireframe(false);

		// Move the camera
		// Maybe set it to orthographic, etc.
		glm::mat4 matAIQuad = glm::mat4(1.0f);
		DrawObject(matAIQuad, pAIQuad, shaderProgID, pTheVAOManager);
	}

	// 3. Use the FBO colour texture as the texture on that quad.
	// set the passNumber to 1
	glUniform1i(passNumber_UniLoc, 2);

	glActiveTexture(GL_TEXTURE0 + 30);
	glBindTexture(GL_TEXTURE_2D, pPlatformFBO->colourTexture_0_ID);

	GLuint texSampPlatformFBO_UL = glGetUniformLocation(shaderProgID, "PlatformColourTexture");
	glUniform1i(texSampPlatformFBO_UL, 30);

	// 4. Draw a single object (a triangle or quad)
	iObject* pPlatformQuad = pFindObjectByFriendlyName("cube");
	if (pPlatformQuad)
	{
		pPlatformQuad->setScale(5.0f);
		pPlatformQuad->setIsVisible(true);
		//glm::vec3 oldLocation = glm::vec3(::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z);
		pPlatformQuad->setPositionXYZ(glm::vec3(::g_pFlyCamera->getAtInWorldSpace().x - 100.0f, ::g_pFlyCamera->getAtInWorldSpace().y, ::g_pFlyCamera->getAtInWorldSpace().z + 300));
		//pQuadOrIsIt->setPositionXYZ(glm::vec3(::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z + 100));
		pPlatformQuad->setIsWireframe(false);

		// Move the camera
		// Maybe set it to orthographic, etc.
		glm::mat4 matPlatformQuad = glm::mat4(1.0f);
		DrawObject(matPlatformQuad, pPlatformQuad, shaderProgID, pTheVAOManager);
	}

	// set pass number back to 0 to render the rest of the scene
	glUniform1i(passNumber_UniLoc, 0);

	for (int index = 0; index != ::g_vec_pCharacterObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pCharacterObjects[index];

		//*******************************
		//	FACE FORWARD

		glm::vec3 currentVelocity;
		pCurrentObject->GetVelocity(currentVelocity);
		if (currentVelocity.x > 0.0f && currentVelocity.y > 0.0f && currentVelocity.z > 0.0f)
		{
			glm::vec3 normalizedVelocity = glm::normalize(currentVelocity);
			//normalizedVelocity.z *= -1.0f;
			glm::quat orientation = glm::quatLookAt(normalizedVelocity, glm::vec3(0.0f, 1.0f, 0.0f));
			orientation.x = 0.0f;
			orientation.y *= -1.0f;
			orientation.z = 0.0f;
			pCurrentObject->setRotationXYZ(orientation);
		}

		//*********************************

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
	if (pMainCharacter)
	{
		pMainCharacter->setAnimation(currentAnimationName);
	}
}