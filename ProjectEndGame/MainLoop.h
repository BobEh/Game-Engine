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

	iObject* pMainCharacter = pFindObjectByFriendlyName("mainCharacter");

	if (pMainCharacter != nullptr && currentAnimationName != "Jump" && currentAnimationName != "Roll" && currentAnimationName != "Dying" && currentAnimationName != "Attack")
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

	physicsWorld->Update(deltaTime);

	if (bLightDebugSheresOn)
	{
		DrawDebugSpheres();
	}// if (bLightDebugSheresOn) 
}

void DrawSecondPass()
{
	// The whole scene is now drawn (to the FBO)

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

	for (int index = 0; index != ::g_vec_pGameObjects.size(); index++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);

		iObject* pCurrentObject = ::g_vec_pGameObjects[index];

		//glm::vec3 resetThePosition = glm::vec3(0.0f, 0.0f, 0.0f);
		//pCurrentObject->GetComponent()->GetPosition(resetThePosition);

		//pCurrentObject->setPositionXYZ(resetThePosition);

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