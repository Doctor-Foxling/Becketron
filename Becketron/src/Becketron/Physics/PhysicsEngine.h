#pragma once

#include "PhysicsObject.h"

class PhysicsEngine
{
public:
	void AddObject(PhysicsObject* object);

	void Simulate(float delta_time);

	// TODO: Temporary Getters
	const PhysicsObject* GetObject(unsigned int index) const
	{
		return m_objects[index];
	}
	inline unsigned int GetNumObjects() const
	{
		return (unsigned int)m_objects.size();
	}

	void HandleCollision();
private:
	std::vector<PhysicsObject*> m_objects;
};