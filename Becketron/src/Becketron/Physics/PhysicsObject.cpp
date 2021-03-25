#include "btpch.h"
#include "PhysicsObject.h"

void PhysicsObject::Integrate(float delta_time)
{
	m_position += m_velocity * delta_time;
}