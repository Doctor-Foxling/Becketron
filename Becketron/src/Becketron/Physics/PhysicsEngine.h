#pragma once

class PhysicsEngine
{
public:
	PhysicsEngine() {}
	virtual ~PhysicsEngine() {}

	virtual bool InitPhysicsEngine() { return true; }
};