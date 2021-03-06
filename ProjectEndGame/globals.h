#pragma once

// GLFW stuff
GLFWwindow* window;
int width, height;

// what should I draw??
enum class renderTag
{
	AI,
	Platform,
	Menu,
	none
};

enum class menuSelection
{
	exit,
	platform,
	galactica,
	overworld
};
bool drawSpace = false;
renderTag currentRender;
menuSelection currentMenuSelection = menuSelection::overworld;

// physics stuff
nPhysics::iPhysicsFactory* physicsFactory;
nPhysics::iPhysicsWorld* physicsWorld;
cPhysics* pAIPhsyics;
cPhysics* pPlatformPhysics;

cBasicTextureManager* g_pTextureManager = NULL;

bool g_MouseIsInsideWindow = false;
bool g_MouseLeftButtonIsDown = false;

bool jumping = false;
bool rolling = false;
bool attacking = false;

iObject* pCurrentObject;

//xml stuff
using namespace pugi;

xml_document document;
std::string gameDataLocation = "gameData.xml";
xml_parse_result result = document.load_file(gameDataLocation.c_str());
std::ofstream file;
xml_node root_node = document.child("GameData");
xml_node lightData = root_node.child("LightData");
xml_node rampData = root_node.child("RampData");
xml_node ballData = root_node.child("BallData");
xml_node ballLightData = root_node.child("BallLightData");

// method declarations
void CalcAABBsForMeshModel(cMesh& theMesh);
void SetUpTextureBindingsForObject(cGameObject* pCurrentObject,	GLint shaderProgID);
void DrawObject(glm::mat4 m, iObject* pCurrentObject, GLint shaderProgID, cVAOManager* pVAOManager);
glm::mat4 calculateWorldMatrix(iObject* pCurrentObject);

// AABB stuff
extern std::map<unsigned long long /*ID*/, cAABB*> g_mapAABBs_World;
bool displayAABBs = false;
cAABB* pCurrentAABBLeft;
cAABB* pCurrentAABBRight;
cAABB* pCurrentAABBFront;
cAABB* pCurrentAABBBack;

// off screen rendering
cFBO* pFullScreenFBO = NULL;
cFBO* pAIFBO = NULL;
cFBO* pPlatformFBO = NULL;
GLint passNumber_UniLoc;
GLint renderFullScreen_UL;
GLint eyeLocation_UL;
GLint shipLevelCompleted_UL;
GLint randomLightContrib_UL;
GLint randomScaling_UL;
GLint platformerCompleted_UL;
GLint renderMenu_UL;
GLint renderMenuNotSelected_UL;
GLint renderMenuSelector_UL;

glm::mat4 p, v, AIv, Pv, FSv;

glm::vec2 waterOffset;

bool fileChanged = false;

unsigned int currentRamp = 0;

// light stuff
cLight* pMainLight = new cLight();
cLight* pMainLight1 = new cLight();
cLight* pMainLight2 = new cLight();
cLight* pMainLight3 = new cLight();
cLight* pMainLight4 = new cLight();
std::vector<cLight*> pLightsVec;
unsigned int currentLight = 0;
cLight* pCorner1Light = new cLight();
cLight* pCorner2Light = new cLight();
cLight* pCorner3Light = new cLight();
cLight* pCorner4Light = new cLight();
bool bLightDebugSheresOn = false;

//camera stuff
cFlyCamera* g_pFlyCamera = NULL;
cFlyCamera* theAICamera = NULL;
cFlyCamera* thePlatformCamera = NULL;
cFlyCamera* theFullScreenCamera = NULL;

// game object vectors
std::vector<iObject*> g_vec_pGameObjects;
std::vector<iObject*> g_vec_pEnvironmentObjects;
std::vector<iObject*> g_vec_pGameFBOObjects;
std::vector<nPhysics::iPhysicsComponent*> g_vec_pGameComponentObjects;
std::vector<iObject*> g_vec_pClothObjects;
std::vector<iObject*> g_vec_pCharacterObjects;
std::vector<iObject*> g_vec_pAIEnvironmentObjects;
std::vector<iObject*> g_vec_pAIEnemyObjects;
std::vector<iObject*> g_vec_pAIGameObjects;
std::vector<iObject*> g_vec_pPlatformEnvironmentObjects;
std::vector<iObject*> g_vec_pPlatformEnemyObjects;
std::vector<iObject*> g_vec_pPlatformGameObjects;
std::vector<iObject*> g_vec_pPlatformCharacterObjects;
std::vector<iObject*> g_vec_pPlatformExplosionObjects;
std::vector<iObject*> g_vec_pExplosionObjects;
std::vector<iObject*> g_vec_pPlaceHolderObjects;
std::vector<iObject*> g_vec_pMenuItemsObjects;
std::map<std::string /*FriendlyName*/, iObject*> g_map_GameObjectsByFriendlyName;

//AI
Coordinator* gCoordinator;
AIManager* gAIManager;

//Platformer Game Stuff
int platformJumpCount = 0;
int platformRollCount = 0;
int platformDyingCount = 0;
int platformAttackCount = 0;

int enemyCount1 = 0;
int enemyCount2 = 0;
int enemyCount3 = 0;
int enemyCount4 = 0;

int enemy1Health = 3;
int enemy2Health = 3;
int enemy3Health = 3;
int enemy4Health = 3;

bool gotHit = false;

bool changeToAI = false;
bool changeToMain = false;
bool changeToPlatform = false;
bool renderMenu = false;

float bufferOffset = 0.0f;

// returns NULL (0) if we didn't find it.
iObject* pFindObjectByFriendlyName(std::string name);
iObject* pFindObjectByFriendlyNameMap(std::string name);

// loading stuff
GLuint shaderProgID;
cVAOManager* pTheVAOManager;
cModelLoader* pTheModelLoader;
cObjectFactory* pFactory;
cLightHelper* pLightHelper;
cLowPassFilter avgDeltaTimeThingy;
double lastTime;
double deltaTime;

//debug objects
iObject* pDebugSphere;
iObject* pDebugCube;
iObject* pSkyBoxSphere;

//global objects
iObject* pMoon;
iObject* pMars;
iObject* pMainShip;
iObject* gPlayerBullet;
iObject* pShipToken;
iObject* pTokenCharacter;

#define NUM_OF_THREADS 1

CRITICAL_SECTION output_lock;

//random function
template <class T>
T randInRange(T min, T max)
{
	double value =
		min + static_cast <double> (rand())
		/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
	return static_cast<T>(value);
};

//gl error function
static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

float transitionTime = 0.0f;
void TransitionToAI(float dt)
{
	if (transitionTime < 3.0f)
	{
		bufferOffset = randInRange(0.0f, 0.5f);
		transitionTime += dt;
		glm::quat tokenRotation = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(10.0f)));
		pShipToken->setRotationXYZ(pShipToken->getRotationXYZ() * tokenRotation);
		return;
	}
	currentRender = renderTag::AI;
	transitionTime = 0.0f;
	bufferOffset = 0.0f;
	iObject* pMainCharacter = pFindObjectByFriendlyName("mainCharacter");
	pMainCharacter->setPositionXYZ(glm::vec3(40.0f, 5.0f, 0.0f));
	pMainCharacter->ApplyForce(glm::vec3(0.0f, 0.0f, -10.0f));
	//g_pFlyCamera->eye = glm::vec3(0.0f, 80.0, -80.0);
	changeToAI = false;
}

void TransitionToPlatform(float dt)
{
	if (transitionTime < 3.0f)
	{
		bufferOffset += randInRange(0.0f, 0.5f);
		transitionTime += dt;
		glm::quat tokenRotation = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(10.0f)));
		pTokenCharacter->setRotationXYZ(pTokenCharacter->getRotationXYZ() * tokenRotation);
		return;
	}
	currentRender = renderTag::Platform;
	transitionTime = 0.0f;
	bufferOffset = 0.0f;
	iObject* pMainCharacter = pFindObjectByFriendlyName("mainCharacter");
	pMainCharacter->setPositionXYZ(glm::vec3(40.0f, 5.0f, 0.0f));
	pMainCharacter->ApplyForce(glm::vec3(0.0f, 0.0f, 10.0f));
	//g_pFlyCamera->eye = glm::vec3(0.0f, 80.0, -80.0);
	changeToPlatform = false;
}