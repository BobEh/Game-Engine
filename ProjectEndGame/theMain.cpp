#include "includes.h"
#include "inputs.h"
#include "LoadScene.h"
#include "UtilityFunctions.h"
#include "MainLoop.h"

int main(void)
{

	//   .----------------.  .----------------.  .----------------.  .----------------.  .----------------. 
	//  | .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |
	//  | |    _______   | || |  _________   | || |  _________   | || | _____  _____ | || |   ______     | |
	//  | |   /  ___  |  | || | |_   ___  |  | || | |  _   _  |  | || ||_   _||_   _|| || |  |_   __ \   | |
	//  | |  |  (__ \_|  | || |   | |_  \_|  | || | |_/ | | \_|  | || |  | |    | |  | || |    | |__) |  | |
	//  | |   '.___`-.   | || |   |  _|  _   | || |     | |      | || |  | '    ' |  | || |    |  ___/   | |
	//  | |  |`\____) |  | || |  _| |___/ |  | || |    _| |_     | || |   \ `--' /   | || |   _| |_      | |
	//  | |  |_______.'  | || | |_________|  | || |   |_____|    | || |    `.__.'    | || |  |_____|     | |
	//  | |              | || |              | || |              | || |              | || |              | |
	//  | '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |
	//   '----------------'  '----------------'  '----------------'  '----------------'  '----------------' 

	int result = LoadPhysics();
	if (result != 0)
	{
		std::cout << "Couldn't load the physics objects" << std::endl;
		return 1;
	}

	pFactory = new cObjectFactory();

	result = LoadGLFW();

	void ProcessAsyncMouse(GLFWwindow * window);
	void ProcessAsyncKeys(GLFWwindow * window);

	cDebugRenderer* pDebugRenderer = new cDebugRenderer();
	pDebugRenderer->initialize();

	LoadMeshes();

	LoadTextures();

	InitializeCriticalSection(&output_lock);
	HANDLE thread_handles[NUM_OF_THREADS];
	DWORD thread_ids[NUM_OF_THREADS];

	thread_handles[0] = CreateThread(NULL, 0, LoadObjects, 0, 0, &thread_ids[0]);
	//LoadObjects();

	LoadLights();

	//set current object
	iObject* pCurrentObject = pFindObjectByFriendlyName("mainCharacter");

	::g_pFlyCamera = new cFlyCamera();
	::g_pFlyCamera->eye = glm::vec3(0.0f, 80.0, -80.0);
	theAICamera = new cFlyCamera();
	theAICamera->eye = glm::vec3(0.0f, 200.0, -50.0);
	thePlatformCamera = new cFlyCamera();
	thePlatformCamera->eye = glm::vec3(-460.0f, 250.0f, -960.0f);

	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing

	//delta time stuff
	lastTime = glfwGetTime();

	//Load the fbo stuff
	pAIFBO = new cFBO();
	std::string AIFBOError;
	if (pAIFBO->init(1080, 1080, AIFBOError))
	{
		std::cout << "AI Frame buffer is OK" << std::endl;
	}
	else
	{
		std::cout << "AI FBO Error: " << AIFBOError << std::endl;
	}

	pPlatformFBO = new cFBO();
	std::string PlatformFBOError;
	if (pPlatformFBO->init(1080, 1080, PlatformFBOError))
	{
		std::cout << "Platform Frame buffer is OK" << std::endl;
	}
	else
	{
		std::cout << "Platform FBO Error: " << PlatformFBOError << std::endl;
	}


	//   .----------------.  .----------------.  .----------------.  .-----------------.  .----------------.  .----------------.  .----------------.  .----------------. 
	//  | .--------------. || .--------------. || .--------------. || .--------------. | | .--------------. || .--------------. || .--------------. || .--------------. |
	//  | | ____    ____ | || |      __      | || |     _____    | || | ____  _____  | | | |   _____      | || |     ____     | || |     ____     | || |   ______     | |
	//  | ||_   \  /   _|| || |     /  \     | || |    |_   _|   | || ||_   \|_   _| | | | |  |_   _|     | || |   .'    `.   | || |   .'    `.   | || |  |_   __ \   | |
	//  | |  |   \/   |  | || |    / /\ \    | || |      | |     | || |  |   \ | |   | | | |    | |       | || |  /  .--.  \  | || |  /  .--.  \  | || |    | |__) |  | |
	//  | |  | |\  /| |  | || |   / ____ \   | || |      | |     | || |  | |\ \| |   | | | |    | |   _   | || |  | |    | |  | || |  | |    | |  | || |    |  ___/   | |
	//  | | _| |_\/_| |_ | || | _/ /    \ \_ | || |     _| |_    | || | _| |_\   |_  | | | |   _| |__/ |  | || |  \  `--'  /  | || |  \  `--'  /  | || |   _| |_      | |
	//  | ||_____||_____|| || ||____|  |____|| || |    |_____|   | || ||_____|\____| | | | |  |________|  | || |   `.____.'   | || |   `.____.'   | || |  |_____|     | |
	//  | |              | || |              | || |              | || |              | | | |              | || |              | || |              | || |              | |
	//  | '--------------' || '--------------' || '--------------' || '--------------' | | '--------------' || '--------------' || '--------------' || '--------------' |
	//   '----------------'  '----------------'  '----------------'  '----------------'   '----------------'  '----------------'  '----------------'  '----------------' 
	currentRender = renderTag::none;

	while (!glfwWindowShouldClose(window))
	{
		SetDeltaTime();

		ProcessAsyncKeys(window);
		ProcessAsyncMouse(window);

		float ratio;
		
		glm::mat4 p, v, AIv, Pv;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// Projection matrix
		p = glm::perspective(0.6f,		// FOV
			ratio,			// Aspect ratio
			0.1f,			// Near clipping plane
			10000000.0f);		// Far clipping plane

		// View matrix
		v = glm::mat4(1.0f);
		AIv = glm::mat4(1.0f);
		Pv = glm::mat4(1.0f);

		if (pCurrentObject)
		{
			v = glm::lookAt(g_pFlyCamera->eye, pCurrentObject->getPositionXYZ(), g_pFlyCamera->getUpVector());
			AIv = glm::lookAt(theAICamera->eye, glm::vec3(0.0f, 0.0f, 0.0f), theAICamera->getUpVector());
			Pv = glm::lookAt(thePlatformCamera->eye, glm::vec3(thePlatformCamera->eye.x, thePlatformCamera->eye.y, 0.0f), thePlatformCamera->getUpVector());
		}
		else
		{
			v = glm::lookAt(g_pFlyCamera->eye, glm::vec3(0.0f,0.0f,0.0f), g_pFlyCamera->getUpVector());
			AIv = glm::lookAt(g_pFlyCamera->eye, glm::vec3(0.0f, 0.0f, 0.0f), g_pFlyCamera->getUpVector());
			Pv = glm::lookAt(g_pFlyCamera->eye, glm::vec3(0.0f, 0.0f, 0.0f), g_pFlyCamera->getUpVector());
		}
		glViewport(0, 0, width, height);

		PlaceLights();

		GLint eyeLocation_UL = glGetUniformLocation(shaderProgID, "eyeLocation");

		glUniform4f(eyeLocation_UL,
			::g_pFlyCamera->eye.x,
			::g_pFlyCamera->eye.y,
			::g_pFlyCamera->eye.z, 1.0f);

		GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
		GLint matAIView_UL = glGetUniformLocation(shaderProgID, "theAIView");
		GLint matPlatformView_UL = glGetUniformLocation(shaderProgID, "thePlatformView");
		GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");

		GLint renderAI_UL = glGetUniformLocation(shaderProgID, "useAI");
		GLint renderPlatform_UL = glGetUniformLocation(shaderProgID, "usePlatform");

		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matAIView_UL, 1, GL_FALSE, glm::value_ptr(AIv));
		glUniformMatrix4fv(matPlatformView_UL, 1, GL_FALSE, glm::value_ptr(Pv));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));



		//             _____   ______ _          _     _____              
		//       /\   |_   _| |  ____(_)        | |   |  __ \             
		//      /  \    | |   | |__   _ _ __ ___| |_  | |__) |_ _ ___ ___ 
		//     / /\ \   | |   |  __| | | '__/ __| __| |  ___/ _` / __/ __|
		//    / ____ \ _| |_  | |    | | |  \__ \ |_  | |  | (_| \__ \__ \
		//   /_/    \_\_____| |_|    |_|_|  |___/\__| |_|   \__,_|___/___/
		//                                                                
		//                                                                

		//AIv = glm::lookAt(theAICamera->eye, glm::vec3(0.0f, 0.0f, 0.0f), theAICamera->getUpVector());
		glUniform1f(renderAI_UL, true);
		DrawAIFBO();
		glUniform1f(renderAI_UL, false);


		//    _____  _       _    __                       ______ _          _     _____              
		//   |  __ \| |     | |  / _|                     |  ____(_)        | |   |  __ \             
		//   | |__) | | __ _| |_| |_ ___  _ __ _ __ ___   | |__   _ _ __ ___| |_  | |__) |_ _ ___ ___ 
		//   |  ___/| |/ _` | __|  _/ _ \| '__| '_ ` _ \  |  __| | | '__/ __| __| |  ___/ _` / __/ __|
		//   | |    | | (_| | |_| || (_) | |  | | | | | | | |    | | |  \__ \ |_  | |  | (_| \__ \__ \
		//   |_|    |_|\__,_|\__|_| \___/|_|  |_| |_| |_| |_|    |_|_|  |___/\__| |_|   \__,_|___/___/
		//                                                                                            
		//
		std::map<std::pair<int, int>, int> colVals;
		int r = 1;
		int c = 1;
		int currentNode = 4;
		colVals.insert(std::make_pair(std::make_pair(c,r),currentNode));
		//Pv = glm::lookAt(thePlatformCamera->eye, glm::vec3(thePlatformCamera->eye.x, thePlatformCamera->eye.y, 0.0f), thePlatformCamera->getUpVector());
		glUniform1f(renderPlatform_UL, true);
		DrawPlatformFBO();
		glUniform1f(renderPlatform_UL, false);

		//     _____                          _   _____              
		//    / ____|                        | | |  __ \             
		//   | (___   ___  ___ ___  _ __   __| | | |__) |_ _ ___ ___ 
		//    \___ \ / _ \/ __/ _ \| '_ \ / _` | |  ___/ _` / __/ __|
		//    ____) |  __/ (_| (_) | | | | (_| | | |  | (_| \__ \__ \
		//   |_____/ \___|\___\___/|_| |_|\__,_| |_|   \__,_|___/___/
		//                                                           
		//   

		if (currentRender == renderTag::AI)
		{
			glUniform1f(renderAI_UL, true);
			DrawAI();
			glUniform1f(renderAI_UL, false);
		}
		else if (currentRender == renderTag::Platform)
		{
			glUniform1f(renderPlatform_UL, true);
			DrawPlatform();
			glUniform1f(renderPlatform_UL, false);
		}
		else if (currentRender == renderTag::none)
		{
			glUniform1f(renderPlatform_UL, false);
			glUniform1f(renderAI_UL, false);
			//g_pFlyCamera->eye = glm::vec3(0.0f, 20.0, -80.0);
			pCurrentObject = pFindObjectByFriendlyName("mainCharacter");
			if (pCurrentObject)
			{
				v = glm::lookAt(g_pFlyCamera->eye, pCurrentObject->getPositionXYZ(), g_pFlyCamera->getUpVector());
			}
			else
			{
				v = glm::lookAt(g_pFlyCamera->eye, glm::vec3(0.0f), g_pFlyCamera->getUpVector());
			}
			DrawSecondPass();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}// main loop

	if (fileChanged)
	{
		//file.open(gameDataLocation);
		file << "<?xml version='1.0' encoding='utf-8'?>\n";
		document.save_file(gameDataLocation.c_str());
		//file.close();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	// Delete everything
	delete pTheModelLoader;
	delete pTheVAOManager;

	exit(EXIT_SUCCESS);
}