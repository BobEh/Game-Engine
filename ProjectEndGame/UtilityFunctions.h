#pragma once
#include "includes.h"
#include "globals.h"

void SetUpTextureBindingsForObject(iObject* pCurrentObject,	GLint shaderProgID)
{

	//// Tie the texture to the texture unit
	//GLuint texSamp0_UL = ::g_pTextureManager->getTextureIDFromName("Pizza.bmp");
	//glActiveTexture(GL_TEXTURE0);				// Texture Unit 0
	//glBindTexture(GL_TEXTURE_2D, texSamp0_UL);	// Texture now assoc with texture unit 0

	// Tie the texture to the texture unit
	GLuint texSamp0_UL = ::g_pTextureManager->getTextureIDFromName(pCurrentObject->getTextures(0));
	glActiveTexture(GL_TEXTURE0);				// Texture Unit 0
	glBindTexture(GL_TEXTURE_2D, texSamp0_UL);	// Texture now assoc with texture unit 0

	GLuint texSamp1_UL = ::g_pTextureManager->getTextureIDFromName(pCurrentObject->getTextures(1));
	glActiveTexture(GL_TEXTURE1);				// Texture Unit 1
	glBindTexture(GL_TEXTURE_2D, texSamp1_UL);	// Texture now assoc with texture unit 0

	GLuint texSamp2_UL = ::g_pTextureManager->getTextureIDFromName(pCurrentObject->getTextures(2));
	glActiveTexture(GL_TEXTURE2);				// Texture Unit 2
	glBindTexture(GL_TEXTURE_2D, texSamp2_UL);	// Texture now assoc with texture unit 0

	GLuint texSamp3_UL = ::g_pTextureManager->getTextureIDFromName(pCurrentObject->getTextures(3));
	glActiveTexture(GL_TEXTURE3);				// Texture Unit 3
	glBindTexture(GL_TEXTURE_2D, texSamp3_UL);	// Texture now assoc with texture unit 0

	// Tie the texture units to the samplers in the shader
	GLint textSamp00_UL = glGetUniformLocation(shaderProgID, "textSamp00");
	glUniform1i(textSamp00_UL, 0);	// Texture unit 0

	GLint textSamp01_UL = glGetUniformLocation(shaderProgID, "textSamp01");
	glUniform1i(textSamp01_UL, 1);	// Texture unit 1

	GLint textSamp02_UL = glGetUniformLocation(shaderProgID, "textSamp02");
	glUniform1i(textSamp02_UL, 2);	// Texture unit 2

	GLint textSamp03_UL = glGetUniformLocation(shaderProgID, "textSamp03");
	glUniform1i(textSamp03_UL, 3);	// Texture unit 3


	GLint tex0_ratio_UL = glGetUniformLocation(shaderProgID, "tex_0_3_ratio");
	glUniform4f(tex0_ratio_UL,
		pCurrentObject->getTextureRatio(0),		// 1.0
		pCurrentObject->getTextureRatio(1),
		pCurrentObject->getTextureRatio(2),
		pCurrentObject->getTextureRatio(3));

	{
		//textureWhatTheWhat
		GLuint texSampWHAT_ID = ::g_pTextureManager->getTextureIDFromName("WhatTheWhat.bmp");
		glActiveTexture(GL_TEXTURE13);				// Texture Unit 13
		glBindTexture(GL_TEXTURE_2D, texSampWHAT_ID);	// Texture now assoc with texture unit 0

		GLint textureWhatTheWhat_UL = glGetUniformLocation(shaderProgID, "textureWhatTheWhat");
		glUniform1i(textureWhatTheWhat_UL, 13);	// Texture unit 13
	}



	return;
}

static glm::vec2 g_OffsetHACK = glm::vec2(0.0f, 0.0f);

float HACK_FrameTime = 0.0f;

void DrawObject(glm::mat4 m, iObject* pCurrentObject, GLint shaderProgID, cVAOManager* pVAOManager)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the texture bindings and samplers
//
//// Tie the texture to the texture unit
//	GLuint grassTexID = ::g_pTextureManager->getTextureIDFromName("grassTexture_512.bmp");
//	glActiveTexture(GL_TEXTURE0);				// Texture Unit 0
//	glBindTexture(GL_TEXTURE_2D, grassTexID);	// Texture now assoc with texture unit 0
//
//	GLuint sandTexID = ::g_pTextureManager->getTextureIDFromName("sandTexture_512.bmp");
//	glActiveTexture(GL_TEXTURE5);				// Texture Unit 5
//	glBindTexture(GL_TEXTURE_2D, sandTexID);	// Texture now assoc with texture unit 0
//
//
//	// Tie the texture units to the samplers in the shader
//	GLint textSamp01_UL = glGetUniformLocation(shaderProgID, "textSamp01");
//	glUniform1i(textSamp01_UL, 0);	// Texture unit 0
//
//	GLint textSamp02_UL = glGetUniformLocation(shaderProgID, "textSamp02");
//	glUniform1i(textSamp02_UL, 5);	// Texture unit 5
	// ************

	//SetUpTextureBindingsForObject(pCurrentObject, shaderProgID);
	GLint bIsSkyBox_UL = glGetUniformLocation(shaderProgID, "bIsSkyBox");

	if (pCurrentObject->getIsVisible() == false)
	{
		return;
	}
	if (pCurrentObject->getFriendlyName() != "skybox")
	{
		if (pCurrentObject->getFriendlyName() == "island")
		{
			GLint seaFloorBool = glGetUniformLocation(shaderProgID, "bIsIsland");
			glUniform1f(seaFloorBool, true);
			GLint seaFloor = glGetUniformLocation(shaderProgID, "seaFloor");
			glUniform1f(seaFloor, 0.1f);
		}
		else
		{
			GLint seaFloorBool = glGetUniformLocation(shaderProgID, "bIsIsland");
			glUniform1f(seaFloorBool, false);
		}
		if (pCurrentObject->getFriendlyName() == "water")
		{
			GLint waterBool = glGetUniformLocation(shaderProgID, "bIsWater");
			glUniform1f(waterBool, true);
			GLint waterOffsetShader = glGetUniformLocation(shaderProgID, "waterOffset");
			glUniform2f(waterOffsetShader, waterOffset.x, waterOffset.y);

			//uniform bool useHeightMap;	// If true, use heightmap
			GLint useHeightMap_UL = glGetUniformLocation(shaderProgID, "useHeightMap");
			glUniform1f(useHeightMap_UL, (float)GL_TRUE);

			//uniform sampler2D heightMap;
	//		GLuint heightMapID = ::g_pTextureManager->getTextureIDFromName("IslandHeightMap.bmp");
			GLuint heightMapID = ::g_pTextureManager->getTextureIDFromName("water_800.bmp");
			const int TEXTURE_UNIT_40 = 40;
			glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_40);				// Texture Unit 18
			glBindTexture(GL_TEXTURE_2D, heightMapID);	// Texture now assoc with texture unit 0

			// Tie the texture units to the samplers in the shader
			GLint heightMap_UL = glGetUniformLocation(shaderProgID, "heightMap");
			glUniform1i(heightMap_UL, TEXTURE_UNIT_40);	// Texture unit 18

			// ANOTHER HACK
			GLint textOffset_UL = glGetUniformLocation(shaderProgID, "textOffset");
			glUniform2f(textOffset_UL, waterOffset.x, waterOffset.y);
		}
		else
		{
			GLint waterBool = glGetUniformLocation(shaderProgID, "bIsWater");
			glUniform1f(waterBool, false);
			GLint useHeightMap_UL = glGetUniformLocation(shaderProgID, "useHeightMap");
			glUniform1f(useHeightMap_UL, (float)GL_FALSE);
		}
		if (pCurrentObject->getFriendlyName() == "seaFloor")
		{
			GLint sand = glGetUniformLocation(shaderProgID, "bIsSand");
			glUniform1f(sand, true);
		}
		else
		{
			GLint sand = glGetUniformLocation(shaderProgID, "bIsSand");
			glUniform1f(sand, false);
		}
		// Is a regular 2D textured object
		SetUpTextureBindingsForObject(pCurrentObject, shaderProgID);
		glUniform1f(bIsSkyBox_UL, (float)GL_FALSE);

		// Don't draw back facing triangles (default)
		glCullFace(GL_BACK);
		//glm::mat4 testing = glm::mat4(1.0f);
		//pCurrentObject->GetComponent()->GetTransform(m);
		//if (testing != glm::mat4(1.0f))
		//{

		//Translation (done in physics engine)
		if (pCurrentObject->getFriendlyName() == "moon")
		{
			//std::cout << "the moon" << std::endl;
		}
		if (pCurrentObject->GetComponent() == nullptr)
		{
			m = calculateWorldMatrix(pCurrentObject);
		}
		if (pCurrentObject->GetComponent() != nullptr)
		{
			if (pCurrentObject->GetComponent()->GetComponentType() != nPhysics::eComponentType::cloth)
			{
				pCurrentObject->GetComponent()->GetTransform(m);

				//Rotation
				glm::mat4 rotation = glm::mat4(pCurrentObject->getRotationXYZ());
				m *= rotation;

				//Scale
				float theScale = pCurrentObject->getScale();
				glm::mat4 scale = glm::scale(glm::mat4(1.0f),
					glm::vec3(theScale));
				m = m * scale;
			}
		}
		else
		{
			m = calculateWorldMatrix(pCurrentObject);
		}
	}
	else
	{
		GLint useHeightMap_UL = glGetUniformLocation(shaderProgID, "useHeightMap");
		glUniform1f(useHeightMap_UL, (float)GL_FALSE);
		GLint seaFloorBool = glGetUniformLocation(shaderProgID, "bIsIsland");
		glUniform1f(seaFloorBool, false);
		// Draw the back facing triangles. 
		// Because we are inside the object, so it will force a draw on the "back" of the sphere 
		glCullFace(GL_FRONT_AND_BACK);

		glUniform1f(bIsSkyBox_UL, (float)GL_TRUE);

		if (renderAI)
		{
			GLuint skyBoxTextureID = ::g_pTextureManager->getTextureIDFromName("space");
			glActiveTexture(GL_TEXTURE10);				// Texture Unit 26
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextureID);	// Texture now assoc with texture unit 0
		}
		else
		{
			GLuint skyBoxTextureID = ::g_pTextureManager->getTextureIDFromName("sunny");
			glActiveTexture(GL_TEXTURE10);				// Texture Unit 26
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextureID);	// Texture now assoc with texture unit 0
		}

		// Tie the texture units to the samplers in the shader
		GLint skyBoxSampler_UL = glGetUniformLocation(shaderProgID, "skyBox");
		glUniform1i(skyBoxSampler_UL, 10);	// Texture unit 26
		m = calculateWorldMatrix(pCurrentObject);
	}

	//m = calculateWorldMatrix(pCurrentObject);


	glUseProgram(shaderProgID);


	//glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
	//glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

	//uniform mat4 matModel;		// Model or World 
	//uniform mat4 matView; 		// View or camera
	//uniform mat4 matProj;
	GLint matModel_UL = glGetUniformLocation(shaderProgID, "matModel");

	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));
	//glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
	//glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));



	// Find the location of the uniform variable newColour
	GLint newColour_location = glGetUniformLocation(shaderProgID, "newColour");

	glUniform3f(newColour_location,
		pCurrentObject->getObjectColourRGBA().r,
		pCurrentObject->getObjectColourRGBA().g,
		pCurrentObject->getObjectColourRGBA().b);

	GLint diffuseColour_UL = glGetUniformLocation(shaderProgID, "diffuseColour");
	glUniform4f(diffuseColour_UL,
		pCurrentObject->getObjectColourRGBA().r,
		pCurrentObject->getObjectColourRGBA().g,
		pCurrentObject->getObjectColourRGBA().b,
		pCurrentObject->getTransprancyValue());	// 

	GLint specularColour_UL = glGetUniformLocation(shaderProgID, "specularColour");
	glUniform4f(specularColour_UL,
		1.0f,	// R
		1.0f,	// G
		1.0f,	// B
		1000.0f);	// Specular "power" (how shinny the object is)
					// 1.0 to really big (10000.0f)


//uniform vec4 debugColour;
//uniform bool bDoNotLight;
	GLint debugColour_UL = glGetUniformLocation(shaderProgID, "debugColour");
	GLint bDoNotLight_UL = glGetUniformLocation(shaderProgID, "bDoNotLight");

	if (pCurrentObject->getIsWireframe())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// LINES
		glUniform4f(debugColour_UL,
			pCurrentObject->getDebugColour().r,
			pCurrentObject->getDebugColour().g,
			pCurrentObject->getDebugColour().b,
			pCurrentObject->getDebugColour().a);
		glUniform1f(bDoNotLight_UL, (float)GL_TRUE);
	}
	else
	{	// Regular object (lit and not wireframe)
		glUniform1f(bDoNotLight_UL, (float)GL_FALSE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
	}
	//glPointSize(15.0f);

	if (pCurrentObject->getDisableDepthBufferTest())
	{
		glDisable(GL_DEPTH_TEST);					// DEPTH Test OFF
	}
	else
	{
		glEnable(GL_DEPTH_TEST);						// Turn ON depth test
	}

	if (pCurrentObject->getDisableDepthBufferWrite())
	{
		glDisable(GL_FALSE);						// DON'T Write to depth buffer
	}
	else
	{
		glEnable(GL_TRUE);								// Write to depth buffer
	}

	GLint isSkinnedMesh_UniLoc = glad_glGetUniformLocation(shaderProgID, "isSkinnedMesh");

	if (pCurrentObject->getSM() != NULL)
	{
		glUniform1f(isSkinnedMesh_UniLoc, (float)GL_TRUE);

		// Set to all identity
		const int NUMBEROFBONES = 100;
		glm::mat4 matBones[NUMBEROFBONES];

		for (int index = 0; index != NUMBEROFBONES; index++)
		{
			matBones[index] = glm::mat4(1.0f);	// Identity
		}

		// Taken from "Skinned Mesh 2 - todo.docx"
		std::vector< glm::mat4x4 > vecFinalTransformation;
		std::vector< glm::mat4x4 > vecOffsets;
		std::vector< glm::mat4x4 > vecObjectBoneTransformation;

		// This loads the bone transforms from the animation model
		pCurrentObject->getSM()->BoneTransform(HACK_FrameTime,	// 0.0f // Frame time
			currentAnimationName,
			vecFinalTransformation,
			vecObjectBoneTransformation,
			vecOffsets);

		// Wait until all threads are done updating.

		if (currentAnimationName == "Walk")
		{
			HACK_FrameTime += 0.009f;
		}
		else if (currentAnimationName == "Jump")
		{
			HACK_FrameTime += 0.003f;
		}
		else if (currentAnimationName == "Roll")
		{
			HACK_FrameTime += 0.004f;
		}
		else if (currentAnimationName == "Dying")
		{
			HACK_FrameTime += 0.001f;
		}
		else
		{
			HACK_FrameTime += 0.008f;
		}


		//{// Forward kinematic stuff

		//	// "Bone" location is at the origin
		//	glm::vec4 boneLocation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		//	// bone #22 is "B_R_Hand" in this model
		//	glm::mat4 matSpecificBone = vecFinalTransformation[9];

		//	// Transformed into "model" space where that bone is.
		//	::g_HACK_vec3_BoneLocationFK = matSpecificBone * boneLocation;

		//	// If it's in world space
		//	::g_HACK_vec3_BoneLocationFK = m * glm::vec4(g_HACK_vec3_BoneLocationFK, 1.0f);

		//	if (currentAnimationName == "Walk" || currentAnimationName == "Walk-Slow")
		//	{
		//		glm::vec3 setPosition = glm::vec3(0.0f, 0.0f, g_HACK_vec3_BoneLocationFK.z);

		//		pCurrentObject->setPositionXYZ(setPosition);
		//	}
		//}// Forward kinematic 


			// Copy all 100 bones to the shader
		GLint matBonesArray_UniLoc = glGetUniformLocation(shaderProgID, "matBonesArray[0]");
		// The "100" is to pass 100 values, starting at the pointer location of matBones[0];
		//glUniformMatrix4fv(matBonesArray_UniLoc, 100, GL_FALSE, glm::value_ptr(matBones[0]));

		GLint numBonesUsed = (GLint)vecFinalTransformation.size();

		glUniformMatrix4fv(matBonesArray_UniLoc, numBonesUsed,
			GL_FALSE,
			glm::value_ptr(vecFinalTransformation[0]));

	}
	else
	{
		glUniform1f(isSkinnedMesh_UniLoc, (float)GL_FALSE);
	}

	//		glDrawArrays(GL_TRIANGLES, 0, 2844);
	//		glDrawArrays(GL_TRIANGLES, 0, numberOfVertsOnGPU);

	sModelDrawInfo drawInfo;
	//if (pTheVAOManager->FindDrawInfoByModelName("bunny", drawInfo))
	if (pVAOManager->FindDrawInfoByModelName(pCurrentObject->getMeshName(), drawInfo))
	{
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,
			drawInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);
	}

	return;
} // DrawObject;
// 

// returns NULL (0) if we didn't find it.
iObject* pFindObjectByFriendlyName(std::string name)
{
	// Do a linear search 
	for (unsigned int index = 0;
		index != g_vec_pGameObjects.size(); index++)
	{
		if (::g_vec_pGameObjects[index]->getFriendlyName() == name)
		{
			// Found it!!
			return ::g_vec_pGameObjects[index];
		}
	}
	for (unsigned int index = 0;
		index != g_vec_pEnvironmentObjects.size(); index++)
	{
		if (::g_vec_pEnvironmentObjects[index]->getFriendlyName() == name)
		{
			// Found it!!
			return ::g_vec_pEnvironmentObjects[index];
		}
	}
	for (unsigned int index = 0;
		index != g_vec_pGameFBOObjects.size(); index++)
	{
		if (::g_vec_pGameFBOObjects[index]->getFriendlyName() == name)
		{
			// Found it!!
			return ::g_vec_pGameFBOObjects[index];
		}
	}
	for (unsigned int index = 0; index != g_vec_pCharacterObjects.size(); index++)
	{
		if (::g_vec_pCharacterObjects[index]->getFriendlyName() == name)
		{
			// Found it!!
			return ::g_vec_pCharacterObjects[index];
		}
	}
	// Didn't find it
	return NULL;
}

// returns NULL (0) if we didn't find it.
iObject* pFindObjectByFriendlyNameMap(std::string name)
{
	//std::map<std::string, cGameObject*> g_map_GameObjectsByFriendlyName;
	return ::g_map_GameObjectsByFriendlyName[name];
}

glm::mat4 calculateWorldMatrix(iObject* pCurrentObject)
{

	glm::mat4 matWorld = glm::mat4(1.0f);


	// ******* TRANSLATION TRANSFORM *********
	glm::vec3 tempPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	//pCurrentObject->GetComponent()->GetPosition(tempPosition);
	glm::mat4 componentTrans;
	//pCurrentObject->GetComponent()->GetTransform(componentTrans);
	componentTrans
		= glm::translate(glm::mat4(1.0f),
			glm::vec3(pCurrentObject->getPositionXYZ().x,
				pCurrentObject->getPositionXYZ().y,
				pCurrentObject->getPositionXYZ().z));
	//std::cout << "In Calculate function X: " << pCurrentObject->getPositionXYZ().x << ", Y: " << pCurrentObject->getPositionXYZ().y << ", Z: " << pCurrentObject->getPositionXYZ().z << std::endl;
	matWorld = matWorld * componentTrans;
	// ******* TRANSLATION TRANSFORM *********



	// ******* ROTATION TRANSFORM *********

	glm::mat4 rotation = glm::mat4(pCurrentObject->getRotationXYZ());
	matWorld *= rotation;
	// ******* ROTATION TRANSFORM *********



	// ******* SCALE TRANSFORM *********
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
		glm::vec3(pCurrentObject->getScale(),
			pCurrentObject->getScale(),
			pCurrentObject->getScale()));
	matWorld = matWorld * scale;
	// ******* SCALE TRANSFORM *********


	return matWorld;
}