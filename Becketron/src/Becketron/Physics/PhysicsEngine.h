#pragma once

#include <PhysX/PxPhysics.h>

class PhysicsEngine
{
public:
	PhysicsEngine() {}
	virtual ~PhysicsEngine() {}

	virtual bool InitPhysicsEngine() { return true; }
};