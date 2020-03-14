#pragma once
#include "includes.h"
#include "globals.h"

HMODULE hGetDLL;
typedef nPhysics::iPhysicsFactory* (*func_createPhysicsFactory)();

int LoadPhysics()
{
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

	cMesh mainCharacterMesh;
	if (!pTheModelLoader->LoadModel_Assimp("assets/modelsFBX/player.md5mesh", mainCharacterMesh, assimpErrorString))
	{
		std::cout << "Error: couldn't find the island ply." << std::endl;
	}

	cMesh seaFloorMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/SeaFloor_xyz_n_uv.ply", seaFloorMesh))
	{
		std::cout << "Error: couldn't find the mountain range ply." << std::endl;
	}

	cMesh waterMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Water_xyz_n_uv.ply", waterMesh))
	{
		std::cout << "Error: couldn't find the mountain range ply." << std::endl;
	}

	cMesh floorMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Floor.ply", floorMesh))
	{
		std::cout << "Error: couldn't find the floor ply." << std::endl;
	}

	cMesh wallRightMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Wall_Right.ply", wallRightMesh))
	{
		std::cout << "Error: couldn't find the floor ply." << std::endl;
	}


	cMesh wallLeftMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Wall_Left.ply", wallLeftMesh))
	{
		std::cout << "Error: couldn't find the floor ply." << std::endl;
	}

	cMesh wallFrontMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Wall_Front.ply", wallFrontMesh))
	{
		std::cout << "Error: couldn't find the floor ply." << std::endl;
	}

	cMesh wallBackMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/Wall_Back.ply", wallBackMesh))
	{
		std::cout << "Error: couldn't find the floor ply." << std::endl;
	}

	cMesh sharkMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/shark_xyz_n_uv.ply", sharkMesh))
	{
		std::cout << "Error: couldn't find the mountain range ply." << std::endl;
	}

	cMesh fishMesh;
	if (!pTheModelLoader->LoadPlyModel("assets/models/fish_xyz_n_uv.ply", fishMesh))
	{
		std::cout << "Error: couldn't find the mountain range ply." << std::endl;
	}


	cMesh cubeMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Cube_1_Unit_from_origin_XYZ_uv.ply", cubeMesh);

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

	sModelDrawInfo cubeMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("cube",
		cubeMesh,		// Sphere mesh info
		cubeMeshInfo,
		shaderProgID);

	sModelDrawInfo mainCharacterMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("mainCharacter", mainCharacterMesh, mainCharacterMeshInfo, shaderProgID);

	sModelDrawInfo seaFloorMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("seaFloor", seaFloorMesh, seaFloorMeshInfo, shaderProgID);

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

	sModelDrawInfo waterMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("water", waterMesh, waterMeshInfo, shaderProgID);

	sModelDrawInfo eagleMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("eagle", eagleMesh, eagleMeshInfo, shaderProgID);

	sModelDrawInfo sharkMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("shark", sharkMesh, sharkMeshInfo, shaderProgID);

	sModelDrawInfo fishMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("fish", fishMesh, fishMeshInfo, shaderProgID);
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

	::g_pTextureManager->Create2DTextureFromBMPFile("water_800.bmp", true);

	//Cube Maps loaded here
	::g_pTextureManager->SetBasePath("assets/textures/cubemaps/");
	std::string errorString;

	//if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("space",
	//													   "TropicalSunnyDayRight2048.bmp", "TropicalSunnyDayLeft2048.bmp",
	//													   "TropicalSunnyDayUp2048.bmp", "TropicalSunnyDayDown2048.bmp",
	//													   "TropicalSunnyDayFront2048.bmp", "TropicalSunnyDayBack2048.bmp", true, errorString ))
	//{

	//first cube map
	/*if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("space",
		"up-the-creek_rt.bmp", "up-the-creek_lf.bmp",
		"up-the-creek_up.bmp", "up-the-creek_dn.bmp",
		"up-the-creek_ft.bmp", "up-the-creek_bk.bmp", true, errorString))
	{
		std::cout << "Space skybox loaded" << std::endl;
	}
	else
	{
		std::cout << "OH NO! " << errorString << std::endl;
	}*/

	//Second cube map
	if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("space",
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

int LoadObjects()
{
	//AI
	gAIManager = new AIManager();
	gCoordinator = new Coordinator();

	//Objects

	iObject* pCloth = pFactory->CreateObject("cloth", nPhysics::eComponentType::cloth);
	pCloth->setMeshName("cube");
	pCloth->setFriendlyName("sphere");	// We use to search 
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
	physicsCloth.NodeMass = 0.6f;
	physicsCloth.NumNodesAcross = 40;
	physicsCloth.NumNodesDown = 20;
	physicsCloth.SpringConstant = 55.0f;
	physicsCloth.NodeRadius = 0.8f;
	nPhysics::iClothComponent* pClothPhysics = physicsFactory->CreateCloth(physicsCloth);
	g_vec_pGameComponentObjects.push_back(pClothPhysics);
	pCloth->SetComponent(pClothPhysics);
	::g_vec_pClothObjects.push_back(pCloth);
	physicsWorld->AddComponent(pCloth->GetComponent());

	iObject* pMainCharacter = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);
	nPhysics::sBallDef characterPhysics;
	characterPhysics.Mass = pMainCharacter->getScale();
	characterPhysics.Position = glm::vec3(0.0f, 50.0f, 0.0f);
	characterPhysics.Radius = 5.0f;
	characterPhysics.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	characterPhysics.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	characterPhysics.Angle = 1.0f;
	nPhysics::iBallComponent* pCharacterPhysics = physicsFactory->CreateBall(characterPhysics);
	g_vec_pGameComponentObjects.push_back(pCharacterPhysics);
	pMainCharacter->SetComponent(pCharacterPhysics);
	cSimpleAssimpSkinnedMesh* theSkinnedMesh = new cSimpleAssimpSkinnedMesh();
	pMainCharacter->setSM(theSkinnedMesh);
	pMainCharacter->getSM()->LoadMeshFromFile("mainCharacter", "assets/modelsFBX/RPG-Character(FBX2013).FBX");

	sModelDrawInfo* mainCharacterMeshInfo = pMainCharacter->getSM()->CreateMeshObjectFromCurrentModel();

	pMainCharacter->getSM()->LoadMeshAnimation("Run", "assets/modelsFBX/Run.fbx");
	pMainCharacter->getSM()->LoadMeshAnimation("Walk-Slow", "assets/modelsFBX/RPG-Character_Unarmed-Walk-Slow(FBX2013).FBX");
	pMainCharacter->getSM()->LoadMeshAnimation("Walk", "assets/modelsFBX/Walking.fbx");
	pMainCharacter->getSM()->LoadMeshAnimation("Jump", "assets/modelsFBX/Jumping.fbx");
	pMainCharacter->getSM()->LoadMeshAnimation("Fall", "assets/modelsFBX/RPG-Character_Unarmed-Fall(FBX2013).FBX");
	pMainCharacter->getSM()->LoadMeshAnimation("Attack", "assets/modelsFBX/RPG-Character_Unarmed-Attack-R3(FBX2013).FBX");
	pMainCharacter->getSM()->LoadMeshAnimation("Idle", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).FBX");
	pMainCharacter->getSM()->LoadMeshAnimation("Dying", "assets/modelsFBX/RPG-Character_Unarmed-Death1(FBX2013).FBX");
	pMainCharacter->getSM()->LoadMeshAnimation("Roll", "assets/modelsFBX/Sprinting Forward Roll.fbx");
	currentAnimationName = "Idle";
	pMainCharacter->setMeshName("mainCharacter");
	pMainCharacter->setFriendlyName("mainCharacter");	// We use to search
	pMainCharacter->setRotationXYZ(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
	pMainCharacter->setScale(0.2f);
	//pMainCharacter->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
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
	//g_vec_pGameObjects.push_back(pMainCharacter);
	physicsWorld->AddComponent(pMainCharacter->GetComponent());
	if (mainCharacterMeshInfo)
	{
		std::cout << mainCharacterMeshInfo->numberOfVertices << " vertices" << std::endl;
		std::cout << mainCharacterMeshInfo->numberOfTriangles << " triangles" << std::endl;
		std::cout << mainCharacterMeshInfo->numberOfIndices << " indices" << std::endl;

		pTheVAOManager->LoadModelDrawInfoIntoVAO(*mainCharacterMeshInfo, shaderProgID);
	}

	// Sphere and cube
	iObject* pSphere = pFactory->CreateObject("sphere", nPhysics::eComponentType::ball);

	pSphere->setMeshName("sphere");
	pSphere->setFriendlyName("sphere");	// We use to search 
	pSphere->setPositionXYZ(glm::vec3(0.0f, 50.0f, 0.0f));
	pSphere->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pSphere->setScale(4.0f);
	pSphere->setObjectColourRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//pSphere->setDebugColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pSphere->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	pSphere->setAccel(glm::vec3(0.0f, 0.0f, 0.0f));
	pSphere->set_SPHERE_radius(1.0f);
	pSphere->setInverseMass(1.0f);
	pSphere->setTexture("StoneTex_1024.bmp", 1);
	pSphere->setTextureRatio(1, 1);
	pSphere->SetPlaneType("floor");
	pSphere->setTransprancyValue(1.0f);
	pSphere->setIsVisible(true);
	pSphere->setIsWireframe(false);
	//pSphere->SetMassType(0);
	nPhysics::sBallDef physicsBall;
	physicsBall.Mass = 4.0f;
	physicsBall.Position = glm::vec3(10.0f, 50.0f, 0.0f);
	physicsBall.Radius = 4.0f;
	physicsBall.Angle = 1.0f;
	physicsBall.Rotation = glm::vec3(1.0f, 1.0f, 1.0f);
	physicsBall.Scale = glm::vec3(4.0f, 4.0f, 4.0f);
	nPhysics::iBallComponent* pSpherePhysics = physicsFactory->CreateBall(physicsBall);
	g_vec_pGameComponentObjects.push_back(pSpherePhysics);
	pSphere->SetComponent(pSpherePhysics);
	::g_vec_pGameObjects.push_back(pSphere);
	physicsWorld->AddComponent(pSphere->GetComponent());

	for (int i = 0; i < 3; i++)
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
	pFloor->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
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
	pDebugCube->setMeshName("cube");
	pDebugCube->setFriendlyName("debug_cube");
	//pDebugCube->setPositionXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pDebugCube->setRotationXYZ(glm::vec3(0.0f, 0.0f, 0.0f));
	pDebugCube->setScale(1.0f);
	//	pDebugSphere->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	pDebugCube->setDebugColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	pDebugCube->setIsWireframe(true);
	pDebugCube->setInverseMass(0.0f);			// Sphere won't move
	pDebugCube->setIsVisible(false);
	nPhysics::sBallDef physicsCubeDebug;
	physicsCubeDebug.Mass = 0.0f;
	physicsCubeDebug.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	physicsCubeDebug.Radius = 1.0f;
	nPhysics::iBallComponent* pDebugCubePhysics = physicsFactory->CreateBall(physicsCubeDebug);
	pDebugCube->SetComponent(pDebugCubePhysics);

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