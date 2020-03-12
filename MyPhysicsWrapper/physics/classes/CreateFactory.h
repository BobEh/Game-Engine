#pragma once
#include "cPhysicsFactory.h"

#define DLL_EXPORT extern "C" __declspec(dllexport)

DLL_EXPORT nPhysics::iPhysicsFactory* createFactory();