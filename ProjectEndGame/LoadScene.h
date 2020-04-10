#pragma once
#include "includes.h"
#include "globals.h"

HMODULE hGetDLL;
typedef nPhysics::iPhysicsFactory* (*func_createPhysicsFactory)();

int LoadPhysics()
{
	pAIPhsyics = new cPhysics();
	pPlatformPhysics = new cPhysics();
	//find the DLL for creating the physics factory
	hGetDLL = LoadLibraryA("MyPhysicsWrapper.dll");
	//hGetDLL = LoadLibraryA("BulletPhysicsWrapper.dll");
	if (!hGetDLL)
	{
		std::cout << "failed to load the library" << std::endl;
		return 1;
	}

	//function pointer to the createFactory method
	func_createPhysicsFactory createFactory = 0;
	createFactory = (func_createPhysicsFactory)GetProcAddress(hGetDLL, "createFactory");

	//create the physics factory
	physicsFactory = createFactory();

	//generate the physics world
	physicsWorld = physicsFactory->CreateWorld();
	if (!physicsWorld)
	{
		std::cout << "Couldn't make physicsWorld" << std::endl;
	}

	return 0;
}

int LoadGLFW()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	// Mouse callbacks
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	return 0;
}

int LoadMeshes()
{
	pTheModelLoader = new cModelLoader();

	std::string assimpErrorString = "";

	cMesh floorMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Floor.ply", floorMesh))
	{
		std::cout << "Error: couldn't find the floor ply." << std::endl;
	}

	cMesh wallRightMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Wall_Right.ply", wallRightMesh))
	{
		std::cout << "Error: couldn't find the wall right ply." << std::endl;
	}


	cMesh wallLeftMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Wall_Left.ply", wallLeftMesh))
	{
		std::cout << "Error: couldn't find the wall left ply." << std::endl;
	}

	cMesh wallFrontMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Wall_Front.ply", wallFrontMesh))
	{
		std::cout << "Error: couldn't find the wall front ply." << std::endl;
	}

	cMesh wallBackMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Wall_Back.ply", wallBackMesh))
	{
		std::cout << "Error: couldn't find the wall back ply." << std::endl;
	}

	cMesh sharkMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/shark_xyz_n_uv.ply", sharkMesh))
	{
		std::cout << "Error: couldn't find the shark ply." << std::endl;
	}

	cMesh fishMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/fish_xyz_n_uv.ply", fishMesh))
	{
		std::cout << "Error: couldn't find the fish ply." << std::endl;
	}

	cMesh xWingMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/X-Wing_Attack_xyz_n_uv.ply", xWingMesh))
	{
		std::cout << "Error: couldn't find the x wing ply." << std::endl;
	}
	cMesh xWingRMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/X-Wing_Attack_Reverse_xyz_n_uv.ply", xWingRMesh))
	{
		std::cout << "Error: couldn't find the x wing reverse ply." << std::endl;
	}

	cMesh bigSphereMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Big_Sphere_Radius_1_XYZ_n_uv.ply", bigSphereMesh))
	{
		std::cout << "Error: couldn't find the big sphere ply." << std::endl;
	}

	cMesh asteroidMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Asteroid.ply", asteroidMesh))
	{
		std::cout << "Error: couldn't find the asteroid ply file." << std::endl;
	}

	cMesh enemyMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Android_Robot_All_xyz_n_uv.ply", enemyMesh);

	cMesh piece1Mesh;
	pTheModelLoader->LoadPlyModel("assets/models/Android_Robot_Head.ply", piece1Mesh);

	cMesh piece2Mesh;
	pTheModelLoader->LoadPlyModel("assets/models/Android_Robot_Left_Arm.ply", piece2Mesh);

	cMesh piece3Mesh;
	pTheModelLoader->LoadPlyModel("assets/models/Android_Robot_Left_Leg.ply", piece3Mesh);

	cMesh piece4Mesh;
	pTheModelLoader->LoadPlyModel("assets/models/Android_Robot_Right_Arm.ply", piece4Mesh);

	cMesh piece5Mesh;
	pTheModelLoader->LoadPlyModel("assets/models/Android_Robot_Right_Leg.ply", piece5Mesh);

	cMesh piece6Mesh;
	pTheModelLoader->LoadPlyModel("assets/models/Android_Robot_Torso.ply", piece6Mesh);


	cMesh cubeMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Cube_1_Unit_from_origin_XYZ_uv.ply", cubeMesh);

	cMesh cubeMeshDebug;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Cube_debug.ply", cubeMeshDebug))
	{
		std::cout << "Error: couldn't find the debug cube ply file." << std::endl;
	}

	cMesh eagleMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Eagle_xyz_n_uv.ply", eagleMesh);

	cMesh sphereMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Sphere_Radius_1_XYZ_n_uv.ply", sphereMesh);

	cShaderManager* pTheShaderManager = new cShaderManager();

	cShaderManager::cShader vertexShad;
	vertexShad.fileName = "assets/shaders/vertexShader01.glsl";

	cShaderManager::cShader fragShader;
	fragShader.fileName = "assets/shaders/fragmentShader01.glsl";

	if (!pTheShaderManager->createProgramFromFile("SimpleShader", vertexShad, fragShader))
	{
		std::cout << "Error: didn't compile the shader" << std::endl;
		std::cout << pTheShaderManager->getLastError();
		return -1;
	}

	shaderProgID = pTheShaderManager->getIDFromFriendlyName("SimpleShader");

	// Create a VAO Manager...
	// #include "cVAOManager.h"  (at the top of your file)
	pTheVAOManager = new cVAOManager();
	sModelDrawInfo sphereMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("sphere",
		sphereMesh,		// Sphere mesh info
		sphereMeshInfo,
		shaderProgID);

	sModelDrawInfo cubeMeshDebugInfo;
	if (!pTheVAOManager->LoadModelIntoVAO("cubeDebug",
		cubeMeshDebug,		// Sphere mesh info
		cubeMeshDebugInfo,
		shaderProgID))
	{
		std::cout << "Error: couldn't load debug cube into VAO." << std::endl;
	}

	sModelDrawInfo cubeMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("cube",
		cubeMesh,		// Sphere mesh info
		cubeMeshInfo,
		shaderProgID);

	sModelDrawInfo xWingRMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("xWingR", xWingRMesh, xWingRMeshInfo, shaderProgID);

	sModelDrawInfo xWingMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("xWing", xWingMesh, xWingMeshInfo, shaderProgID);

	sModelDrawInfo asteroidMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("asteroid", asteroidMesh, asteroidMeshInfo, shaderProgID);

	sModelDrawInfo bigSphereMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("bigSphere",
		bigSphereMesh,		// Sphere mesh info
		bigSphereMeshInfo,
		shaderProgID);

	sModelDrawInfo floorMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("floor", floorMesh, floorMeshInfo, shaderProgID);

	sModelDrawInfo wallLeftMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("wallLeft", wallLeftMesh, wallLeftMeshInfo, shaderProgID);

	sModelDrawInfo wallRightMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("wallRight", wallRightMesh, wallRightMeshInfo, shaderProgID);

	sModelDrawInfo wallBackMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("wallBack", wallBackMesh, wallBackMeshInfo, shaderProgID);

	sModelDrawInfo wallFrontMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("wallFront", wallFrontMesh, wallFrontMeshInfo, shaderProgID);

	sModelDrawInfo eagleMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("eagle", eagleMesh, eagleMeshInfo, shaderProgID);

	sModelDrawInfo sharkMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("shark", sharkMesh, sharkMeshInfo, shaderProgID);

	sModelDrawInfo fishMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("fish", fishMesh, fishMeshInfo, shaderProgID);


	sModelDrawInfo enemyMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("enemy", enemyMesh, enemyMeshInfo, shaderProgID);

	sModelDrawInfo piece1MeshInfo;
	pTheVAOManager->LoadModelIntoVAO("piece1", piece1Mesh, piece1MeshInfo, shaderProgID);

	sModelDrawInfo piece2MeshInfo;
	pTheVAOManager->LoadModelIntoVAO("piece2", piece2Mesh, piece2MeshInfo, shaderProgID);

	sModelDrawInfo piece3MeshInfo;
	pTheVAOManager->LoadModelIntoVAO("piece3", piece3Mesh, piece3MeshInfo, shaderProgID);

	sModelDrawInfo piece4MeshInfo;
	pTheVAOManager->LoadModelIntoVAO("piece4", piece4Mesh, piece4MeshInfo, shaderProgID);

	sModelDrawInfo piece5MeshInfo;
	pTheVAOManager->LoadModelIntoVAO("piece5", piece5Mesh, piece5MeshInfo, shaderProgID);

	sModelDrawInfo piece6MeshInfo;
	pTheVAOManager->LoadModelIntoVAO("piece6", piece6Mesh, piece6MeshInfo, shaderProgID);

}

int LoadTextures()
{
	// now texture
// Texture stuff
	::g_pTextureManager = new cBasicTextureManager();
	::g_pTextureManager->SetBasePath("assets/textures");

	if (!::g_pTextureManager->Create2DTextureFromBMPFile("grassTexture_512.bmp", true))
	{
		std::cout << "Didn't load texture" << std::endl;
	}

	::g_pTextureManager->Create2DTextureFromBMPFile("StoneTex_1024.bmp", true);

	GLint ID = ::g_pTextureManager->getTextureIDFromName("grassTexture_512.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("sandTexture_1024.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("shark.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("fish.bmp", true);

	g_pTextureManager->Create2DTextureFromBMPFile("mars.bmp", true);

	g_pTextureManager->Create2DTextureFromBMPFile("moon.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("water_800.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("purple.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("red.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("white.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("blue.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("green.bmp", true);
	g_pTextureManager->Create2DTextureFromBMPFile("X-Wing-Texture_bit.bmp", true);

	//Cube Maps loaded here
	::g_pTextureManager->SetBasePath("assets/textures/cubemaps/");
	std::string errorString;

	//if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("space",
	//													   "TropicalSunnyDayRight2048.bmp", "TropicalSunnyDayLeft2048.bmp",
	//													   "TropicalSunnyDayUp2048.bmp", "TropicalSunnyDayDown2048.bmp",
	//													   "TropicalSunnyDayFront2048.bmp", "TropicalSunnyDayBack2048.bmp", true, errorString ))
	//{

	//first cube map
	if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("space",
		"SpaceBox_left2_negX.bmp", "SpaceBox_right1_posX.bmp",
		"SpaceBox_bottom4_negY.bmp", "SpaceBox_top3_posY.bmp",
		"SpaceBox_front5_posZ.bmp", "SpaceBox_back6_negZ.bmp", true, errorString))
	{
		std::cout << "space skybox loaded" << std::endl;
	}
	else
	{
		std::cout << "oh no! " << errorString << std::endl;
	}

	//Second cube map
	if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("sunny",
		"TropicalSunnyDayLeft2048.bmp", "TropicalSunnyDayRight2048.bmp",
		"TropicalSunnyDayDown2048.bmp", "TropicalSunnyDayUp2048.bmp",
		"TropicalSunnyDayFront2048.bmp", "TropicalSunnyDayBack2048.bmp", true, errorString))
	{
		std::cout << "space skybox loaded" << std::endl;
	}
	else
	{
		std::cout << "oh no! " << errorString << std::endl;
	}
	return 0;
}

void LoadLevel()
{
	float size = 20.0f;
	for (int i = 0; i < 17; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3((i * size) * -1.0f, 0.0f, 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}
	float startPoint = (17 * size) + (5 * size);
	for (int i = 0; i < 23; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3((startPoint + (i * size)) * -1.0f, 0.0f, 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}

	for (int i = 1; i < 18; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3(0.0f, (i * size), 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}

	for (int i = 25; i < 26; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3(0.0f, (i * size), 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}

	for (int i = 6; i < 26; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3(-880.0f, (i * size), 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}

	for (int i = 0; i < 22; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3((i * size) * -1.0f, 100.0f, 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}

	for (int i = 0; i < 17; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3((i * size) * -1.0f, 360.0f, 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}

	for (int i = 7; i < 10; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3((i * size) * -1.0f, 180.0f, 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}

	for (int i = 21; i < 22; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3((i * size) * -1.0f, 360.0f, 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}

	for (int i = 26; i < 44; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3((i * size) * -1.0f, 360.0f, 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}

	for (int i = 21; i < 22; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3((i * size) * -1.0f, 210.0f, 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}

	for (int i = 26; i < 44; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3((i * size) * -1.0f, 210.0f, 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}

	for (int i = 32; i < 36; i++)
	{
		iObject* pCube = pFactory->CreateObject("mesh", nPhysics::eComponentType::ball);
		pCube->setMeshName("cube");
		std::string cubeName = "cube" + i;
		pCube->setFriendlyName(cubeName);
		pCube->setPositionXYZ(glm::vec3((i * size) * -1.0f, 110.0f, 0.0f));
		pCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pCube->setScale(1.0f);
		//	pCube->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		pCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pCube->setIsWireframe(false);
		pCube->setInverseMass(0.0f);			// Sphere won't move
		pCube->setIsVisible(true);
		pCube->setTexture("StoneTex_1024.bmp", 1);
		pCube->setTextureRatio(1, 1);
		pCube->setTransprancyValue(1.0f);
		g_vec_pPlatformEnvironmentObjects.push_back(pCube);
	}
}

DWORD WINAPI LoadObjects(LPVOID params)
{
	EnterCriticalSection(&output_lock);
	//AI
	gAIManager = new AIManager();
	gCoordinator = new Coordinator();

	//Objects
	iObject* pCloth = pFactory->CreateObject("cloth", nPhysics::eComponentType::cloth);
	pCloth->setMeshName("sphere");
	pCloth->setFriendlyName("cloth");	// We use to search 
	pCloth->setPositionXYZ(glm::vec3(0.0f, 50.0f, 0.0f));
	pCloth->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pCloth->setScale(0.2f);
	pCloth->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//pCloth->setDebugColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pCloth->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	pCloth->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pCloth->set_SPHERE_radius(1.0f);
	pCloth->setInverseMass(1.0f);
	pCloth->setTexture("StoneTex_1024.bmp", 1);
	pCloth->setTextureRatio(1, 1);
	pCloth->SetPlaneType("floor");
	pCloth->setTransprancyValue(1.0f);
	pCloth->setIsVisible(true);
	pCloth->setIsWireframe(false);
	//pSphere->SetMassType(0);
	nPhysics::sClothDef physicsCloth;
	physicsCloth.CornerA = glm::vec3(5.0f, 20.0f, 0.0f);
	physicsCloth.CornerB = glm::vec3(-20.0f, 20.0f, 0.0f);
	physicsCloth.DownDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	physicsCloth.NodeMass = 0.4f;
	physicsCloth.NumNodesAcross = 40;
	physicsCloth.NumNodesDown = 20;
	physicsCloth.SpringConstant = 25.0f;
	physicsCloth.NodeRadius = 1.0f;
	nPhysics::iClothComponent* pClothPhysics = physicsFactory->CreateCloth(physicsCloth);
	g_vec_pGameComponentObjects.push_back(pClothPhysics);
	pCloth->SetComponent(pClothPhysics);
	cMesh clothMesh;
	size_t numNodes = pClothPhysics->NumNodes();
	sPlyVertexXYZ_N_UV vertex;
	for (size_t idx = 0; idx < numNodes; idx++)
	{		
		glm::vec3 triPosition = glm::vec3(0.0f);
		pClothPhysics->GetNodePosition(idx, triPosition);
		vertex.x = triPosition.x;
		vertex.y = triPosition.y;
		vertex.z = triPosition.z;
		clothMesh.vecVertices.push_back(vertex);
	}
	sModelDrawInfo clothMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("clothMesh", clothMesh, clothMeshInfo, shaderProgID);
	//pCloth->setMeshName("clothMesh");
	::g_vec_pClothObjects.push_back(pCloth);
	physicsWorld->AddComponent(pCloth->GetComponent());

	iObject* pMainCharacter = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);	
	nPhysics::sBallDef characterPhysics;
	characterPhysics.Mass = 1.0;
	characterPhysics.Position = glm::vec3(30.0f, 50.0f, 0.0f);
	characterPhysics.Radius = 5.0f;
	characterPhysics.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	characterPhysics.Scale = glm::vec3(0.05f, 0.05f, 0.05f);
	characterPhysics.Angle = 1.0f;
	nPhysics::iBallComponent* pCharacterPhysics = physicsFactory->CreateBall(characterPhysics);
	g_vec_pGameComponentObjects.push_back(pCharacterPhysics);
	pMainCharacter->SetComponent(pCharacterPhysics);
	cSimpleAssimpSkinnedMesh* theSkinnedMesh = new cSimpleAssimpSkinnedMesh();
	pMainCharacter->setSM(theSkinnedMesh);
	pMainCharacter->getSM()->LoadMeshFromFile("mainCharacter", "assets/modelsFBX/RPG-Character(FBX2013).FBX");

	sModelDrawInfo* mainCharacterMeshInfo = pMainCharacter->getSM()->CreateMeshObjectFromCurrentModel();
	if (mainCharacterMeshInfo)
	{
		std::cout << mainCharacterMeshInfo->numberOfVertices << " vertices" << std::endl;
		std::cout << mainCharacterMeshInfo->numberOfTriangles << " triangles" << std::endl;
		std::cout << mainCharacterMeshInfo->numberOfIndices << " indices" << std::endl;

		pTheVAOManager->LoadModelDrawInfoIntoVAO(*mainCharacterMeshInfo, shaderProgID);
	}

	pMainCharacter->getSM()->LoadMeshAnimation("Run", "assets/modelsFBX/Run.fbx");
	pMainCharacter->getSM()->LoadMeshAnimation("Walk-Slow", "assets/modelsFBX/RPG-Character_Unarmed-Walk-Slow(FBX2013).FBX");
	pMainCharacter->getSM()->LoadMeshAnimation("Walk", "assets/modelsFBX/Walking.fbx");
	pMainCharacter->getSM()->LoadMeshAnimation("Jump", "assets/modelsFBX/Jumping.fbx");
	pMainCharacter->getSM()->LoadMeshAnimation("Fall", "assets/modelsFBX/RPG-Character_Unarmed-Fall(FBX2013).FBX");
	pMainCharacter->getSM()->LoadMeshAnimation("Attack", "assets/modelsFBX/RPG-Character_Unarmed-Attack-R3(FBX2013).FBX");
	pMainCharacter->getSM()->LoadMeshAnimation("Idle", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).FBX");
	pMainCharacter->getSM()->LoadMeshAnimation("Dying", "assets/modelsFBX/RPG-Character_Unarmed-Death1(FBX2013).FBX");
	pMainCharacter->getSM()->LoadMeshAnimation("Roll", "assets/modelsFBX/Sprinting Forward Roll.fbx");
	pMainCharacter->setAnimation("Idle");
	pMainCharacter->setMeshName("mainCharacter");
	pMainCharacter->setFriendlyName("mainCharacter");	// We use to search
	pMainCharacter->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pMainCharacter->setScale(0.2f);
	pMainCharacter->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pMainCharacter->setDebugColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pMainCharacter->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	pMainCharacter->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pMainCharacter->setInverseMass(1.0f);
	pMainCharacter->setIsVisible(true);
	pMainCharacter->setIsWireframe(false);
	pMainCharacter->setTexture("StoneTex_1024.bmp", 0);
	pMainCharacter->setTexture("grassTexture_512.bmp", 1);
	pMainCharacter->setTexture("sandTexture_1024.bmp", 2);
	pMainCharacter->setTextureRatio(1, 1);
	pMainCharacter->setTransprancyValue(1.0f);
	g_vec_pCharacterObjects.push_back(pMainCharacter);
	physicsWorld->AddComponent(pMainCharacter->GetComponent());

	// Sphere and cube
	iObject* pSphere = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);

	pSphere->setMeshName("sphere");
	pSphere->setFriendlyName("physicsSphere");	// We use to search 
	pSphere->setPositionXYZ(glm::vec3(0.0f, 50.0f, 0.0f));
	pSphere->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pSphere->setScale(1.0f);
	pSphere->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//pSphere->setDebugColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pSphere->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	pSphere->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pSphere->set_SPHERE_radius(1.0f);
	pSphere->setInverseMass(1.0f);
	pSphere->setTexture("StoneTex_1024.bmp", 1);
	pSphere->setTextureRatio(1, 1);
	//pSphere->SetPlaneType("floor");
	pSphere->setTransprancyValue(1.0f);
	pSphere->setIsVisible(true);
	pSphere->setIsWireframe(false);
	//pSphere->SetMassType(0);
	nPhysics::sBallDef physicsBall;
	physicsBall.Mass = 1.0f;
	physicsBall.Position = glm::vec3(20.0f, 50.0f, -30.0f);
	physicsBall.Radius = 1.0f;
	physicsBall.Angle = 1.0f;
	physicsBall.Rotation = glm::vec3(1.0f, 1.0f, 1.0f);
	physicsBall.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	nPhysics::iBallComponent* pSpherePhysics = physicsFactory->CreateBall(physicsBall);
	g_vec_pGameComponentObjects.push_back(pSpherePhysics);
	pSphere->SetComponent(pSpherePhysics);
	::g_vec_pGameObjects.push_back(pSphere);
	physicsWorld->AddComponent(pSphere->GetComponent());

	for (int i = 0; i < 0; i++)
	{
		iObject* pAnotherSphere = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
		float scaleRadius = randInRange(1.0f, 5.0f);
		pAnotherSphere->setMeshName("sphere");
		std::string theName = "sphere" + i;
		pAnotherSphere->setFriendlyName(theName);	// We use to search 
		//pSphere->setPositionXYZ(glm::vec3(0.0f, 50.0f, 0.0f));
		pAnotherSphere->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pAnotherSphere->setScale(scaleRadius);
		pAnotherSphere->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//pSphere->setDebugColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pAnotherSphere->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		pAnotherSphere->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
		pAnotherSphere->set_SPHERE_radius(1.0f);
		pAnotherSphere->setInverseMass(1.0f);
		pAnotherSphere->setTexture("StoneTex_1024.bmp", 1);
		pAnotherSphere->setTextureRatio(1, 1);
		pAnotherSphere->SetPlaneType("floor");
		pAnotherSphere->setTransprancyValue(1.0f);
		pAnotherSphere->setIsVisible(true);
		pAnotherSphere->setIsWireframe(false);
		//pAnotherSphere->SetMassType(0);
		nPhysics::sBallDef physicsAnotherBall;
		physicsAnotherBall.Mass = scaleRadius;
		physicsAnotherBall.Position = glm::vec3(randInRange(-45.0f, 45.0f), randInRange(40.0f, 60.0f), randInRange(-45.0f, 45.0f));
		physicsAnotherBall.Radius = scaleRadius;
		physicsAnotherBall.Angle = 1.0f;
		physicsAnotherBall.Rotation = glm::vec3(scaleRadius, scaleRadius, scaleRadius);
		physicsAnotherBall.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		nPhysics::iBallComponent* pSpherePhysics = physicsFactory->CreateBall(physicsAnotherBall);
		g_vec_pGameComponentObjects.push_back(pSpherePhysics);
		pAnotherSphere->SetComponent(pSpherePhysics);
		::g_vec_pGameObjects.push_back(pAnotherSphere);
		physicsWorld->AddComponent(pAnotherSphere->GetComponent());
	}

	iObject* pWallLeft = pFactory->CreateObject("mesh", nPhysics::eComponentType::plane);
	pWallLeft->setMeshName("wallLeft");
	pWallLeft->setFriendlyName("wallLeft");	// We use to search 
	//pFloor->setPositionXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallLeft->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallLeft->setScale(1.0f);
	pWallLeft->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pWallLeft->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallLeft->set_SPHERE_radius(1.0f);
	pWallLeft->setInverseMass(1.0f);
	pWallLeft->setIsVisible(false);
	pWallLeft->setIsWireframe(false);
	pWallLeft->setTexture("grassTexture_512.bmp", 1);
	pWallLeft->setTextureRatio(1, 1);
	pWallLeft->SetPlaneType("floor");
	pWallLeft->setTransprancyValue(0.1f);
	nPhysics::sPlaneDef physicsPlaneWallLeft;
	physicsPlaneWallLeft.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
	physicsPlaneWallLeft.Position = glm::vec3(-50.0f, 0.0f, 0.0f);
	physicsPlaneWallLeft.Constant = glm::dot(physicsPlaneWallLeft.Normal, physicsPlaneWallLeft.Position);
	physicsPlaneWallLeft.Mass = 0.0f;
	nPhysics::iPlaneComponent* pWallLeftPhysics = physicsFactory->CreatePlane(physicsPlaneWallLeft);
	g_vec_pGameComponentObjects.push_back(pWallLeftPhysics);
	pWallLeft->SetComponent(pWallLeftPhysics);
	//pWallLeft->SetMassType(1);
	::g_vec_pGameObjects.push_back(pWallLeft);
	physicsWorld->AddComponent(pWallLeft->GetComponent());

	iObject* pWallRight = pFactory->CreateObject("mesh", nPhysics::eComponentType::plane);
	pWallRight->setMeshName("wallRight");
	pWallRight->setFriendlyName("wallRight");	// We use to search 
	//pFloor->setPositionXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallRight->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallRight->setScale(1.0f);
	pWallRight->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pWallRight->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallRight->set_SPHERE_radius(1.0f);
	pWallRight->setInverseMass(1.0f);
	pWallRight->setIsVisible(false);
	pWallRight->setIsWireframe(false);
	pWallRight->setTexture("grassTexture_512.bmp", 1);
	pWallRight->setTextureRatio(1, 1);
	pWallRight->SetPlaneType("floor");
	pWallRight->setTransprancyValue(0.1f);
	nPhysics::sPlaneDef physicsPlaneWallRight;
	physicsPlaneWallRight.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	physicsPlaneWallRight.Position = glm::vec3(50.0f, 0.0f, 0.0f);
	physicsPlaneWallRight.Constant = glm::dot(physicsPlaneWallRight.Normal, physicsPlaneWallRight.Position);
	physicsPlaneWallRight.Mass = 0.0f;
	nPhysics::iPlaneComponent* pWallRightPhysics = physicsFactory->CreatePlane(physicsPlaneWallRight);
	g_vec_pGameComponentObjects.push_back(pWallRightPhysics);
	pWallRight->SetComponent(pWallRightPhysics);
	//pWallRight->SetMassType(1);
	::g_vec_pGameObjects.push_back(pWallRight);
	physicsWorld->AddComponent(pWallRight->GetComponent());

	iObject* pWallFront = pFactory->CreateObject("mesh", nPhysics::eComponentType::plane);
	pWallFront->setMeshName("wallFront");
	pWallFront->setFriendlyName("wallFront");	// We use to search 
	//pFloor->setPositionXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallFront->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallFront->setScale(1.0f);
	pWallFront->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pWallFront->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallFront->set_SPHERE_radius(1.0f);
	pWallFront->setInverseMass(1.0f);
	pWallFront->setIsVisible(false);
	pWallFront->setIsWireframe(false);
	pWallFront->setTexture("grassTexture_512.bmp", 1);
	pWallFront->setTextureRatio(1, 1);
	pWallFront->SetPlaneType("floor");
	pWallFront->setTransprancyValue(0.1f);
	nPhysics::sPlaneDef physicsPlaneWallFront;
	physicsPlaneWallFront.Normal = glm::vec3(0.0f, 0.0f, -1.0f);
	physicsPlaneWallFront.Position = glm::vec3(0.0f, 0.0f, 50.0f);
	physicsPlaneWallFront.Constant = glm::dot(physicsPlaneWallFront.Normal, physicsPlaneWallFront.Position);
	physicsPlaneWallFront.Mass = 0.0f;
	nPhysics::iPlaneComponent* pWallFrontPhysics = physicsFactory->CreatePlane(physicsPlaneWallFront);
	g_vec_pGameComponentObjects.push_back(pWallFrontPhysics);
	pWallFront->SetComponent(pWallFrontPhysics);
	//pWallFront->SetMassType(1);
	::g_vec_pGameObjects.push_back(pWallFront);
	physicsWorld->AddComponent(pWallFront->GetComponent());

	iObject* pWallBack = pFactory->CreateObject("mesh", nPhysics::eComponentType::plane);
	pWallBack->setMeshName("wallBack");
	pWallBack->setFriendlyName("wallBack");	// We use to search 
	//pFloor->setPositionXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallBack->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallBack->setScale(1.0f);
	pWallBack->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pWallBack->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pWallBack->set_SPHERE_radius(1.0f);
	pWallBack->setInverseMass(1.0f);
	pWallBack->setIsVisible(false);
	pWallBack->setIsWireframe(false);
	pWallBack->setTexture("grassTexture_512.bmp", 1);
	pWallBack->setTextureRatio(1, 1);
	pWallBack->SetPlaneType("floor");
	pWallBack->setTransprancyValue(0.1f);
	nPhysics::sPlaneDef physicsPlaneWallBack;
	physicsPlaneWallBack.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	physicsPlaneWallBack.Position = glm::vec3(0.0f, 0.0f, -50.0f);
	physicsPlaneWallBack.Constant = glm::dot(physicsPlaneWallBack.Normal, physicsPlaneWallBack.Position);
	physicsPlaneWallBack.Mass = 0.0f;
	nPhysics::iPlaneComponent* pWallBackPhysics = physicsFactory->CreatePlane(physicsPlaneWallBack);
	g_vec_pGameComponentObjects.push_back(pWallBackPhysics);
	pWallBack->SetComponent(pWallBackPhysics);
	//pWallBack->SetMassType(1);
	::g_vec_pGameObjects.push_back(pWallBack);
	physicsWorld->AddComponent(pWallBack->GetComponent());

	iObject* pFloor = pFactory->CreateObject("mesh", nPhysics::eComponentType::plane);
	pFloor->setMeshName("floor");
	pFloor->setFriendlyName("floor");	// We use to search 
	pFloor->setPositionXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pFloor->setRotationXYZ(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));
	pFloor->setScale(1.0f);
	pFloor->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pFloor->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pFloor->set_SPHERE_radius(1.0f);
	pFloor->setInverseMass(1.0f);
	pFloor->setIsVisible(true);
	pFloor->setIsWireframe(false);
	pFloor->setTexture("grassTexture_512.bmp", 1);
	pFloor->setTextureRatio(1, 1);
	pFloor->SetPlaneType("floor");
	pFloor->setTransprancyValue(1.0f);
	nPhysics::sPlaneDef physicsPlane;
	physicsPlane.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	physicsPlane.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	physicsPlane.Constant = glm::dot(physicsPlane.Normal, physicsPlane.Position);
	physicsPlane.Mass = 0.0f;
	physicsPlane.Angle = 1.0f;
	physicsPlane.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	physicsPlane.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	nPhysics::iPlaneComponent* pFloorPhysics = physicsFactory->CreatePlane(physicsPlane);
	g_vec_pGameComponentObjects.push_back(pFloorPhysics);
	pFloor->SetComponent(pFloorPhysics);
	//pFloor->SetMassType(1);
	::g_vec_pGameObjects.push_back(pFloor);
	physicsWorld->AddComponent(pFloor->GetComponent());

	// Will be moved placed around the scene
	pDebugSphere = pFactory->CreateObject("sphere", nPhysics::eComponentType::plane);
	pDebugSphere->setMeshName("sphere");
	pDebugSphere->setFriendlyName("debug_sphere");
	//pDebugSphere->setPositionXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pDebugSphere->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pDebugSphere->setScale(0.1f);
	//	pDebugSphere->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	pDebugSphere->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	pDebugSphere->setIsWireframe(true);
	pDebugSphere->setInverseMass(0.0f);			// Sphere won't move
	pDebugSphere->setIsVisible(false);
	nPhysics::sBallDef physicsBallDebug;
	physicsBallDebug.Mass = 0.0f;
	physicsBallDebug.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	physicsBallDebug.Radius = 1.0f;
	nPhysics::iBallComponent* pDebugPhysics = physicsFactory->CreateBall(physicsBallDebug);
	pDebugSphere->SetComponent(pDebugPhysics);

	pDebugCube = pFactory->CreateObject("sphere", nPhysics::eComponentType::plane);
	pDebugCube->setMeshName("cubeDebug");
	pDebugCube->setFriendlyName("debug_cube");
	//pDebugCube->setPositionXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pDebugCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pDebugCube->setScale(1.0f);
	//	pDebugSphere->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	pDebugCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	pDebugCube->setIsWireframe(true);
	pDebugCube->setInverseMass(0.0f);			// Sphere won't move
	pDebugCube->setIsVisible(false);

	g_vec_pGameFBOObjects.push_back(pDebugCube);

	// "SkyBox"
	pSkyBoxSphere = pFactory->CreateObject("mesh", nPhysics::eComponentType::plane);
	pSkyBoxSphere->setMeshName("sphere");
	pSkyBoxSphere->setFriendlyName("skybox");
	//pSkyBoxSphere->setPositionXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pSkyBoxSphere->setDebugColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pSkyBoxSphere->setIsWireframe(false);
	pSkyBoxSphere->setScale(5000.0f);		// 1.0 to 10,000,000
	//pSkyBoxSphere->isWireframe = true;
	//pSkyBoxSphere->debugColour = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
	//pSkyBoxSphere->setTexture("Pizza.bmp",1);
	//pSkyBoxSphere->textureRatio[0] = 1.0f;
	pSkyBoxSphere->setPhysicsShapeType("SPHERE");
	pSkyBoxSphere->setInverseMass(0.0f);
	nPhysics::sBallDef physicsBallSky;
	physicsBallSky.Mass = 0.0f;
	physicsBallSky.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	physicsBallSky.Radius = 5000.0f;
	nPhysics::iBallComponent* pSkyPhysics = physicsFactory->CreateBall(physicsBallSky);
	g_vec_pGameComponentObjects.push_back(pSkyPhysics);
	pSkyBoxSphere->SetComponent(pSkyPhysics);
	g_vec_pGameObjects.push_back(pSkyBoxSphere);
	g_vec_pAIEnvironmentObjects.push_back(pSkyBoxSphere);
	g_vec_pPlatformEnvironmentObjects.push_back(pSkyBoxSphere);

	//physicsWorld->SetGravity(glm::vec3(0.0f,10.0f,0.0f));

	//iObject* pDebugCube = pFactory->CreateObject("mesh");
	//pDebugCube->setMeshName("sphere");
	//pDebugCube->setFriendlyName("debug_cube");
	//pDebugCube->setPositionXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	//pDebugCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	//pDebugCube->setScale(10.0f);
	//pDebugCube->setDebugColour(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//pDebugCube->setIsWireframe(true);
	//pDebugCube->setInverseMass(0.0f);			// Sphere won't move
	//pDebugCube->setIsVisible(false);

	pMoon = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
	pMoon->setMeshName("bigSphere");
	pMoon->setFriendlyName("moon");	// We use to search 
	pMoon->setPositionXYZ(glm::vec3(-100.0f, -300.0f, -95.0f));
	pMoon->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pMoon->setScale(1.0f);
	pMoon->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//pSphere->setDebugColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pMoon->setTexture("moon.bmp", 1);
	pMoon->setTextureRatio(1, 1);
	pMoon->setTransprancyValue(1.0f);
	pMoon->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	pMoon->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pMoon->set_SPHERE_radius(1.0f);
	pMoon->setInverseMass(1.0f);
	pMoon->setIsVisible(true);
	pMoon->setIsWireframe(false);
	//nPhysics::sBallDef physicsMoon;
	//physicsMoon.Mass = 0.0f;
	//physicsMoon.Position = glm::vec3(-100.0f, -300.0f, -95.0f);
	//physicsMoon.Radius = 1.0f;
	//physicsMoon.Angle = 1.0f;
	//physicsMoon.Rotation = glm::vec3(1.0f, 1.0f, 1.0f);
	//physicsMoon.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	//nPhysics::iBallComponent* pMoonPhysics = physicsFactory->CreateBall(physicsMoon);
	//g_vec_pGameComponentObjects.push_back(pMoonPhysics);
	//pMoon->SetComponent(pMoonPhysics);
	g_vec_pAIEnvironmentObjects.push_back(pMoon);
	//physicsWorld->AddComponent(pMoon->GetComponent());

	pMars = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
	pMars->setMeshName("bigSphere");
	pMars->setFriendlyName("mars");	// We use to search 
	pMars->setPositionXYZ(glm::vec3(200.0f, -600.0f, 300.0f));
	pMars->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pMars->setScale(1.0f);
	pMars->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//pSphere->setDebugColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pMars->setTexture("mars.bmp", 1);
	pMars->setTextureRatio(1, 1);
	pMars->setTransprancyValue(1.0f);
	pMars->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	pMars->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pMars->set_SPHERE_radius(1.0f);
	pMars->setInverseMass(1.0f);
	pMars->setIsVisible(true);
	pMars->setIsWireframe(false);
	//nPhysics::sBallDef physicsMars;
	//physicsMars.Mass = 0.0f;
	//physicsMars.Position = glm::vec3(-100.0f, -300.0f, -95.0f);
	//physicsMars.Radius = 1.0f;
	//physicsMars.Angle = 1.0f;
	//physicsMars.Rotation = glm::vec3(1.0f, 1.0f, 1.0f);
	//physicsMars.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	//nPhysics::iBallComponent* pMarsPhysics = physicsFactory->CreateBall(physicsMars);
	//g_vec_pGameComponentObjects.push_back(pMarsPhysics);
	//pMars->SetComponent(pMarsPhysics);
	g_vec_pAIEnvironmentObjects.push_back(pMars);
	//physicsWorld->AddComponent(pMars->GetComponent());

	gPlayerBullet = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
	gPlayerBullet->setMeshName("sphere");
	gPlayerBullet->setFriendlyName("bullet");	// We use to search 
	gPlayerBullet->setPositionXYZ(glm::vec3(0.0f, 10.0f, -100.0f));
	gPlayerBullet->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	gPlayerBullet->setScale(1.0f);
	//gPlayerBullet->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	gPlayerBullet->setDebugColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	gPlayerBullet->setTexture("red.bmp", 1);
	gPlayerBullet->setTextureRatio(1, 1);
	gPlayerBullet->setTransprancyValue(1.0f);
	gPlayerBullet->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	gPlayerBullet->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	gPlayerBullet->set_SPHERE_radius(1.0f);
	gPlayerBullet->setInverseMass(1.0f);
	gPlayerBullet->setIsVisible(false);
	gPlayerBullet->setIsWireframe(false);
	//nPhysics::sBallDef physicsBullet;
	//physicsBullet.Mass = 1.0f;
	//physicsBullet.Position = glm::vec3(0.0f, 0.0f, -100.0f);
	//physicsBullet.Radius = 1.0f;
	//physicsBullet.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	//nPhysics::iBallComponent* pBulletPhyisics = physicsFactory->CreateBall(physicsBullet);
	//g_vec_pGameComponentObjects.push_back(pBulletPhyisics);
	//gPlayerBullet->SetComponent(pBulletPhyisics);
	g_vec_pAIGameObjects.push_back(gPlayerBullet);
	//physicsWorld->AddComponent(gPlayerBullet->GetComponent());

	// Sphere and cube
	pMainShip = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
	pMainShip->setMeshName("xWing");
	pMainShip->setFriendlyName("mainXWing");	// We use to search 
	pMainShip->setPositionXYZ(glm::vec3(0.0f, 10.0f, 0.0f));
	pMainShip->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pMainShip->setScale(1.0f);
	pMainShip->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pMainShip->setTexture("X-Wing-Texture_bit.bmp", 1);
	pMainShip->setTextureRatio(1, 1);
	pMainShip->setTransprancyValue(1.0f);
	pMainShip->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	pMainShip->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pMainShip->set_SPHERE_radius(1.0f);
	pMainShip->setInverseMass(1.0f);
	pMainShip->setIsVisible(true);
	pMainShip->setIsWireframe(false);
	::g_vec_pAIGameObjects.push_back(pMainShip);

	cPhysics* pPhsyics = new cPhysics();

	for (int i = 0; i < 9; i++)
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
		std::string friendlyName = "enemy" + i;
		pEnemy->setFriendlyName(friendlyName);	// We use to search 
		pEnemy->setPositionXYZ(glm::vec3(randInRange(-100.0f, 100.0f), 10.0f, randInRange(100.0f, 175.0f)));
		pEnemy->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
		pEnemy->setScale(1.0f);
		if (pEnemy->getBehaviour() == "seek")
		{
			Seek* seekBehaviour = new Seek(pEnemy, pSphere);
			gAIManager->SetBehaviour(pEnemy, seekBehaviour);
			pEnemy->setTexture("red.bmp", 1);
		}
		if (pEnemy->getBehaviour() == "pursue")
		{
			Pursue* seekBehaviour = new Pursue(pEnemy, pSphere);
			gAIManager->SetBehaviour(pEnemy, seekBehaviour);
			pEnemy->setTexture("purple.bmp", 1);
		}
		if (pEnemy->getBehaviour() == "approach")
		{
			Approach* seekBehaviour = new Approach(pEnemy, pSphere);
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

	iObject* pPlatformCharacter = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
	//nPhysics::sBallDef platformCharacterPhysics;
	//platformCharacterPhysics.Mass = 1.0;
	//platformCharacterPhysics.Position = glm::vec3(-680.0, 120.0f, 0.0f);
	//platformCharacterPhysics.Radius = 5.0f;
	//platformCharacterPhysics.Rotation = glm::vec3(0.0f, glm::radians(-90.0f), 0.0f);
	//platformCharacterPhysics.Scale = glm::vec3(0.2f, 0.2f, 0.2f);
	//platformCharacterPhysics.Angle = 1.0f;
	//nPhysics::iBallComponent* pPlatformCharacterPhysics = physicsFactory->CreateBall(platformCharacterPhysics);
	//g_vec_pGameComponentObjects.push_back(pPlatformCharacterPhysics);
	//pPlatformCharacter->SetComponent(pPlatformCharacterPhysics);
	cSimpleAssimpSkinnedMesh* platformSkinnedMesh = new cSimpleAssimpSkinnedMesh();
	pPlatformCharacter->setSM(platformSkinnedMesh);
	pPlatformCharacter->getSM()->LoadMeshFromFile("platformCharacter", "assets/modelsFBX/RPG-Character(FBX2013)2.FBX");

	sModelDrawInfo* platformCharacterMeshInfo = pPlatformCharacter->getSM()->CreateMeshObjectFromCurrentModel();

	pPlatformCharacter->getSM()->LoadMeshAnimation("Run", "assets/modelsFBX/Run.fbx");
	pPlatformCharacter->getSM()->LoadMeshAnimation("Walk-Slow", "assets/modelsFBX/RPG-Character_Unarmed-Walk-Slow(FBX2013).FBX");
	pPlatformCharacter->getSM()->LoadMeshAnimation("Walk", "assets/modelsFBX/Walking.fbx");
	pPlatformCharacter->getSM()->LoadMeshAnimation("Jump", "assets/modelsFBX/Jumping.fbx");
	pPlatformCharacter->getSM()->LoadMeshAnimation("Fall", "assets/modelsFBX/RPG-Character_Unarmed-Fall(FBX2013).FBX");
	pPlatformCharacter->getSM()->LoadMeshAnimation("Attack", "assets/modelsFBX/RPG-Character_Unarmed-Attack-R3(FBX2013).FBX");
	pPlatformCharacter->getSM()->LoadMeshAnimation("Idle", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).FBX");
	pPlatformCharacter->getSM()->LoadMeshAnimation("Dying", "assets/modelsFBX/RPG-Character_Unarmed-Death1(FBX2013).FBX");
	pPlatformCharacter->getSM()->LoadMeshAnimation("Roll", "assets/modelsFBX/Sprinting Forward Roll.fbx");
	pPlatformCharacter->setAnimation("Idle");
	pPlatformCharacter->setMeshName("platformCharacter");
	pPlatformCharacter->setFriendlyName("platformCharacter");	// We use to search
	pPlatformCharacter->setPositionXYZ(glm::vec3(-680.0, 120.0f, 0.0f));
	pPlatformCharacter->setRotationXYZ(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
	pPlatformCharacter->setScale(0.2f);
	pPlatformCharacter->setDebugColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pPlatformCharacter->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	pPlatformCharacter->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pPlatformCharacter->setInverseMass(1.0f);
	pPlatformCharacter->setIsVisible(true);
	pPlatformCharacter->setIsWireframe(false);
	pPlatformCharacter->setTexture("StoneTex_1024.bmp", 0);
	pPlatformCharacter->setTexture("grassTexture_512.bmp", 1);
	pPlatformCharacter->setTexture("sandTexture_1024.bmp", 2);
	pPlatformCharacter->setTextureRatio(1, 1);
	pPlatformCharacter->setTransprancyValue(1.0f);
	g_vec_pPlatformCharacterObjects.push_back(pPlatformCharacter);
	//physicsWorld->AddComponent(pPlatformCharacter->GetComponent());
	if (platformCharacterMeshInfo)
	{
		std::cout << platformCharacterMeshInfo->numberOfVertices << " vertices" << std::endl;
		std::cout << platformCharacterMeshInfo->numberOfTriangles << " triangles" << std::endl;
		std::cout << platformCharacterMeshInfo->numberOfIndices << " indices" << std::endl;

		pTheVAOManager->LoadModelDrawInfoIntoVAO(*platformCharacterMeshInfo, shaderProgID);
	}

	iObject* pEnemy1 = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
	pEnemy1->setMeshName("enemy");
	pEnemy1->setFriendlyName("enemy1");	// We use to search 
	pEnemy1->setPositionXYZ(glm::vec3(-30.0, 10.0f, 0.0f));
	pEnemy1->setRotationXYZ(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
	pEnemy1->setScale(1.0f);
	//pEnemy1->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pEnemy1->setDebugColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pEnemy1->setVelocity(glm::vec3(0.0f, 0.0f, 20.0f));
	pEnemy1->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pEnemy1->setInverseMass(1.0f);
	pEnemy1->setIsVisible(true);
	pEnemy1->setIsWireframe(false);
	pEnemy1->setTexture("green.bmp", 1);
	pEnemy1->setTextureRatio(1, 1);
	pEnemy1->setTransprancyValue(1.0f);
	::g_vec_pPlatformEnemyObjects.push_back(pEnemy1);

	iObject* pEnemy2 = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
	pEnemy2->setMeshName("enemy");
	pEnemy2->setFriendlyName("enemy2");	// We use to search 
	pEnemy2->setPositionXYZ(glm::vec3(-60.0, 110.0f, 0.0f));
	pEnemy2->setRotationXYZ(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
	pEnemy2->setScale(1.0f);
	//pEnemy2->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pEnemy2->setDebugColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pEnemy2->setVelocity(glm::vec3(0.0f, 0.0f, 20.0f));
	pEnemy2->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pEnemy2->setInverseMass(1.0f);
	pEnemy2->setIsVisible(true);
	pEnemy2->setIsWireframe(false);
	pEnemy2->setTexture("green.bmp", 1);
	pEnemy2->setTextureRatio(1, 1);
	pEnemy2->setTransprancyValue(1.0f);
	::g_vec_pPlatformEnemyObjects.push_back(pEnemy2);

	iObject* pEnemy3 = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
	pEnemy3->setMeshName("enemy");
	pEnemy3->setFriendlyName("enemy3");	// We use to search 
	pEnemy3->setPositionXYZ(glm::vec3(-540.0, 220.0f, 0.0f));
	pEnemy3->setRotationXYZ(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
	pEnemy3->setScale(1.0f);
	//pEnemy3->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pEnemy3->setDebugColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pEnemy3->setVelocity(glm::vec3(0.0f, 0.0f, 20.0f));
	pEnemy3->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pEnemy3->setInverseMass(1.0f);
	pEnemy3->setIsVisible(true);
	pEnemy3->setIsWireframe(false);
	pEnemy3->setTexture("green.bmp", 1);
	pEnemy3->setTextureRatio(1, 1);
	pEnemy3->setTransprancyValue(1.0f);
	::g_vec_pPlatformEnemyObjects.push_back(pEnemy3);

	iObject* pEnemy4 = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
	pEnemy4->setMeshName("enemy");
	pEnemy4->setFriendlyName("enemy4");	// We use to search 
	pEnemy4->setPositionXYZ(glm::vec3(-540.0, 370.0f, 0.0f));
	pEnemy4->setRotationXYZ(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
	pEnemy4->setScale(1.0f);
	//pEnemy4->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pEnemy4->setDebugColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pEnemy4->setVelocity(glm::vec3(0.0f, 0.0f, 20.0f));
	pEnemy4->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pEnemy4->setInverseMass(1.0f);
	pEnemy4->setIsVisible(true);
	pEnemy4->setIsWireframe(false);
	pEnemy4->setTexture("green.bmp", 1);
	pEnemy4->setTextureRatio(1, 1);
	pEnemy4->setTransprancyValue(1.0f);
	::g_vec_pPlatformEnemyObjects.push_back(pEnemy4);

	LoadLevel();

	pCurrentObject = pFindObjectByFriendlyName("mainCharacter");

	LeaveCriticalSection(&output_lock);

	return 0;
}

int LoadLights()
{
	pLightHelper = new cLightHelper();

	//Get data from xml to set positions of main light
	pMainLight->setNodeName("MainLight");
	xml_node mainLightNode = lightData.child("MainLight");
	pMainLight->setPositionX(std::stof(mainLightNode.child("PositionX").child_value()));
	pMainLight->setPositionY(std::stof(mainLightNode.child("PositionY").child_value()));
	pMainLight->setPositionZ(std::stof(mainLightNode.child("PositionZ").child_value()));
	pMainLight->setPositionXYZ(glm::vec3(std::stof(mainLightNode.child("PositionX").child_value()), std::stof(mainLightNode.child("PositionY").child_value()), std::stof(mainLightNode.child("PositionZ").child_value())));
	pMainLight->setConstAtten(std::stof(mainLightNode.child("ConstAtten").child_value()));
	pMainLight->setLinearAtten(std::stof(mainLightNode.child("LinearAtten").child_value()));
	pMainLight->setQuadraticAtten(std::stof(mainLightNode.child("QuadraticAtten").child_value()));
	pMainLight->setInnerSpot(std::stof(mainLightNode.child("SpotInnerAngle").child_value()));
	pMainLight->setOuterSpot(std::stof(mainLightNode.child("SpotOuterAngle").child_value()));

	pMainLight1->setNodeName("MainLight1");
	xml_node mainLight1Node = lightData.child("MainLight1");
	pMainLight1->setPositionX(std::stof(mainLight1Node.child("PositionX").child_value()));
	pMainLight1->setPositionY(std::stof(mainLight1Node.child("PositionY").child_value()));
	pMainLight1->setPositionZ(std::stof(mainLight1Node.child("PositionZ").child_value()));
	pMainLight1->setPositionXYZ(glm::vec3(std::stof(mainLight1Node.child("PositionX").child_value()), std::stof(mainLight1Node.child("PositionY").child_value()), std::stof(mainLight1Node.child("PositionZ").child_value())));
	pMainLight1->setConstAtten(std::stof(mainLight1Node.child("ConstAtten").child_value()));
	pMainLight1->setLinearAtten(std::stof(mainLight1Node.child("LinearAtten").child_value()));
	pMainLight1->setQuadraticAtten(std::stof(mainLight1Node.child("QuadraticAtten").child_value()));
	pMainLight1->setInnerSpot(std::stof(mainLight1Node.child("SpotInnerAngle").child_value()));
	pMainLight1->setOuterSpot(std::stof(mainLight1Node.child("SpotOuterAngle").child_value()));

	//pMainLight1->setNodeName("SharkSpotLight");
	//pMainLight1->setPositionX(30.0f);
	//pMainLight1->setPositionY(50.0f);
	//pMainLight1->setPositionZ(std::stof(mainLight1Node.child("PositionZ").child_value()));
	//pMainLight1->setPositionXYZ(glm::vec3(std::stof(mainLight1Node.child("PositionX").child_value()), std::stof(mainLight1Node.child("PositionY").child_value()), std::stof(mainLight1Node.child("PositionZ").child_value())));
	//pMainLight1->setConstAtten(std::stof(mainLight1Node.child("ConstAtten").child_value()));
	//pMainLight1->setLinearAtten(std::stof(mainLight1Node.child("LinearAtten").child_value()));
	//pMainLight1->setQuadraticAtten(std::stof(mainLight1Node.child("QuadraticAtten").child_value()));
	//pMainLight1->setInnerSpot(std::stof(mainLight1Node.child("SpotInnerAngle").child_value()));
	//pMainLight1->setOuterSpot(std::stof(mainLight1Node.child("SpotOuterAngle").child_value()));

	pLightsVec.push_back(pMainLight);
	pLightsVec.push_back(pMainLight1);
	return 0;
}

int DrawDebugSpheres()
{
	{// Draw where the light is at
		for (int i = 0; i < pLightsVec.size(); ++i)
		{
			glm::mat4 matModel = glm::mat4(1.0f);
			pDebugSphere->setPositionXYZ(pLightsVec.at(i)->getPositionXYZ());
			pDebugSphere->setScale(0.5f);
			pDebugSphere->setDebugColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			pDebugSphere->setIsWireframe(true);
			DrawObject(matModel, pDebugSphere,
				shaderProgID, pTheVAOManager);
			pDebugSphere->setIsVisible(true);
		}
	}

	// Draw spheres to represent the attenuation...
	{   // Draw a sphere at 1% brightness
		glm::mat4 matModel = glm::mat4(1.0f);
		pDebugSphere->setPositionXYZ(pLightsVec.at(currentLight)->getPositionXYZ());
		float sphereSize = pLightHelper->calcApproxDistFromAtten(
			0.01f,		// 1% brightness (essentially black)
			0.001f,		// Within 0.1%  
			100000.0f,	// Will quit when it's at this distance
			pLightsVec.at(currentLight)->getConstAtten(),
			pLightsVec.at(currentLight)->getLinearAtten(),
			pLightsVec.at(currentLight)->getQuadraticAtten());
		pDebugSphere->setScale(sphereSize);
		pDebugSphere->setDebugColour(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		pDebugSphere->setIsWireframe(true);
		DrawObject(matModel, pDebugSphere,
			shaderProgID, pTheVAOManager);
	}
	{   // Draw a sphere at 25% brightness
		glm::mat4 matModel = glm::mat4(1.0f);
		pDebugSphere->setPositionXYZ(pLightsVec.at(currentLight)->getPositionXYZ());
		float sphereSize = pLightHelper->calcApproxDistFromAtten(
			0.25f,		// 1% brightness (essentially black)
			0.001f,		// Within 0.1%  
			100000.0f,	// Will quit when it's at this distance
			pLightsVec.at(currentLight)->getConstAtten(),
			pLightsVec.at(currentLight)->getLinearAtten(),
			pLightsVec.at(currentLight)->getQuadraticAtten());
		pDebugSphere->setScale(sphereSize);
		pDebugSphere->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pDebugSphere->setIsWireframe(true);
		DrawObject(matModel, pDebugSphere,
			shaderProgID, pTheVAOManager);
	}
	{   // Draw a sphere at 50% brightness
		glm::mat4 matModel = glm::mat4(1.0f);
		pDebugSphere->setPositionXYZ(pLightsVec.at(currentLight)->getPositionXYZ());
		float sphereSize = pLightHelper->calcApproxDistFromAtten(
			0.50f,		// 1% brightness (essentially black)
			0.001f,		// Within 0.1%  
			100000.0f,	// Will quit when it's at this distance
			pLightsVec.at(currentLight)->getConstAtten(),
			pLightsVec.at(currentLight)->getLinearAtten(),
			pLightsVec.at(currentLight)->getQuadraticAtten());
		pDebugSphere->setScale(sphereSize);
		pDebugSphere->setDebugColour(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
		pDebugSphere->setIsWireframe(true);
		DrawObject(matModel, pDebugSphere,
			shaderProgID, pTheVAOManager);
	}
	{   // Draw a sphere at 75% brightness
		glm::mat4 matModel = glm::mat4(1.0f);
		pDebugSphere->setPositionXYZ(pLightsVec.at(currentLight)->getPositionXYZ());
		float sphereSize = pLightHelper->calcApproxDistFromAtten(
			0.75f,		// 1% brightness (essentially black)
			0.001f,		// Within 0.1%  
			100000.0f,	// Will quit when it's at this distance
			pLightsVec.at(currentLight)->getConstAtten(),
			pLightsVec.at(currentLight)->getLinearAtten(),
			pLightsVec.at(currentLight)->getQuadraticAtten());
		pDebugSphere->setScale(sphereSize);
		pDebugSphere->setDebugColour(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		pDebugSphere->setIsWireframe(true);
		DrawObject(matModel, pDebugSphere,
			shaderProgID, pTheVAOManager);
	}
	{   // Draw a sphere at 95% brightness
		glm::mat4 matModel = glm::mat4(1.0f);
		pDebugSphere->setPositionXYZ(pLightsVec.at(currentLight)->getPositionXYZ());
		float sphereSize = pLightHelper->calcApproxDistFromAtten(
			0.95f,		// 1% brightness (essentially black)
			0.001f,		// Within 0.1%  
			100000.0f,	// Will quit when it's at this distance
			pLightsVec.at(currentLight)->getConstAtten(),
			pLightsVec.at(currentLight)->getLinearAtten(),
			pLightsVec.at(currentLight)->getQuadraticAtten());
		pDebugSphere->setScale(sphereSize);
		pDebugSphere->setDebugColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		pDebugSphere->setIsWireframe(true);
		DrawObject(matModel, pDebugSphere,
			shaderProgID, pTheVAOManager);
	}
	return 0;
}