#include "btpch.h"

#include "PhysicsEngine.h"

void PhysicsEngine::AddObject(const PhysicsObject& object)
{
	m_objects.push_back(object);
}

void PhysicsEngine::Simulate(float delta_time)
{
	for (unsigned int i = 0; i < m_objects.size(); i++)
	{
		m_objects[i].Integrate(delta_time);
	}
}