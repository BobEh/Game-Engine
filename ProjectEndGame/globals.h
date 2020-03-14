#pragma once

// GLFW stuff
GLFWwindow* window;
int width, height;

// physics stuff
nPhysics::iPhysicsFactory* physicsFactory;
nPhysics::iPhysicsWorld* physicsWorld;

cBasicTextureManager* g_pTextureManager = NULL;

bool g_MouseIsInsideWindow = false;
bool g_MouseLeftButtonIsDown = false;

bool jumping = false;
bool rolling = false;
bool attacking = false;

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
cFBO* pTheFBO = NULL;
GLint passNumber_UniLoc;

std::string currentAnimationName;

glm::vec2 waterOffset;

bool fileChanged = false;

unsigned int currentRamp = 0;

// light stuff
cLight* pMainLight = new cLight();
cLight* pMainLight1 = new cLight();
std::vector<cLight*> pLightsVec;
unsigned int currentLight = 0;
cLight* pCorner1Light = new cLight();
cLight* pCorner2Light = new cLight();
cLight* pCorner3Light = new cLight();
cLight* pCorner4Light = new cLight();
bool bLightDebugSheresOn = false;

//camera stuff
cFlyCamera* g_pFlyCamera = NULL;
float cameraLeftRight = 0.0f;
glm::vec3 cameraEye;
glm::vec3 cameraTarget;
glm::vec3 upVector;

// game object vectors
std::vector<iObject*> g_vec_pGameObjects;
std::vector<iObject*> g_vec_pEnvironmentObjects;
std::vector<iObject*> g_vec_pGameFBOObjects;
std::vector<nPhysics::iPhysicsComponent*> g_vec_pGameComponentObjects;
std::vector<iObject*> g_vec_pClothObjects;
std::vector<iObject*> g_vec_pCharacterObjects;
std::map<std::string /*FriendlyName*/, iObject*> g_map_GameObjectsByFriendlyName;

//AI
Coordinator* gCoordinator;
AIManager* gAIManager;

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