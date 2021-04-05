#pragma once

#include <PxScene.h>

class PhysicsEngine
{
public:
	PhysicsEngine() {}
	virtual ~PhysicsEngine() {}

	virtual bool InitPhysicsEngine() { return true; }
};