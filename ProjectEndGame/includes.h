#pragma once

//glm stuff
#include "GLCommon.h"
#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>		// c libs
#include <stdio.h>		// c libs

#include <iostream>		// C++ IO standard stuff
#include <map>			// Map aka "dictonary" 

#include "cModelLoader.h"	
#include "cVAOManager.h"		// NEW
//#include "cGameObject.h"

#include "cShaderManager.h"

#include <sstream>
#include <fstream>

#include <limits.h>
#include <float.h>

// The Physics function
#include "PhysicsStuff.h"
#include "cPhysics.h"

#include "DebugRenderer/cDebugRenderer.h"
#include <pugixml/pugixml.hpp>
#include <pugixml/pugixml.cpp>
#include "cLight.h"
#include "cMediator.h"
#include "cObjectFactory.h"
#include "AABBStuff.h"
#include "LowPassFilter.h"
#include "cFBO.h"

//physics stuff
#include <physics/interfaces/iBallComponent.h>
#include <physics/interfaces/eComponentType.h>
#include <physics/interfaces/iCollisionListener.h>
#include <physics/interfaces/iDebugRenderer.h>
#include <physics/interfaces/iPhysicsComponent.h>
#include <physics/interfaces/iPhysicsFactory.h>
#include <physics/interfaces/iPhysicsWorld.h>
#include <physics/interfaces/iPlaneComponent2.h>
#include <physics/interfaces/iClothComponent.h>

//Animation
#include "cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

//AI
#include "AIManager.h"
#include "Coordinator.h"
#include "Seek.h"
#include "Pursue.h"
#include "Approach.h"
#include "Flee.h"
#include "Wander.h"
#include "Evade.h"

// Used to visualize the attenuation of the lights...
#include "LightManager/cLightHelper.h"

// Adding Textures now
#include "cBasicTextureManager.h"

// Adding fly Camera
#include "cFlyCamera.h"

#include "globals.h"