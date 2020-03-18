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

	LoadObjects();

	LoadLights();

	//set current object
	iObject* pCurrentObject = pFindObjectByFriendlyName("mainCharacter");

	::g_pFlyCamera = new cFlyCamera();
	::g_pFlyCamera->eye = glm::vec3(0.0f, 80.0, -80.0);

	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing

	//delta time stuff
	lastTime = glfwGetTime();

	//Load the fbo stuff
	pTheFBO = new cFBO();
	std::string FBOError;
	if (pTheFBO->init(1080, 1080, FBOError))
	{
		std::cout << "Frame buffer is OK" << std::endl;
	}
	else
	{
		std::cout << "FBO Error: " << FBOError << std::endl;
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


	while (!glfwWindowShouldClose(window))
	{
		SetDeltaTime();

		ProcessAsyncKeys(window);
		ProcessAsyncMouse(window);

		float ratio;
		
		glm::mat4 p, v;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// Projection matrix
		p = glm::perspective(0.6f,		// FOV
			ratio,			// Aspect ratio
			0.1f,			// Near clipping plane
			10000000.0f);		// Far clipping plane

		// View matrix
		v = glm::mat4(1.0f);\

		v = glm::lookAt(g_pFlyCamera->eye, pCurrentObject->getPositionXYZ(), g_pFlyCamera->getUpVector());

		glViewport(0, 0, width, height);

		PlaceLights();

		GLint eyeLocation_UL = glGetUniformLocation(shaderProgID, "eyeLocation");

		glUniform4f(eyeLocation_UL,
			::g_pFlyCamera->eye.x,
			::g_pFlyCamera->eye.y,
			::g_pFlyCamera->eye.z, 1.0f);

		GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
		GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");

		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));

		//    ______ _          _     _____              
		//   |  ____(_)        | |   |  __ \             
		//   | |__   _ _ __ ___| |_  | |__) |_ _ ___ ___ 
		//   |  __| | | '__/ __| __| |  ___/ _` / __/ __|
		//   | |    | | |  \__ \ |_  | |  | (_| \__ \__ \
		//   |_|    |_|_|  |___/\__| |_|   \__,_|___/___/
		//                                               
		//                                               

		DrawFirstPass();

		//     _____                          _   _____              
		//    / ____|                        | | |  __ \             
		//   | (___   ___  ___ ___  _ __   __| | | |__) |_ _ ___ ___ 
		//    \___ \ / _ \/ __/ _ \| '_ \ / _` | |  ___/ _` / __/ __|
		//    ____) |  __/ (_| (_) | | | | (_| | | |  | (_| \__ \__ \
		//   |_____/ \___|\___\___/|_| |_|\__,_| |_|   \__,_|___/___/
		//                                                           
		//                                                           

		DrawSecondPass();

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