#include "btpch.h"
#include "PhysicsObject.h"

void PhysicsObject::Integrate(float delta_time)
{
	if (m_playLast != m_play)
	{
		m_playLast = m_play;
		// Rest to originally assigned values
		m_position = m_InitialPosition;
		m_velocity = m_InitialVelocity;
		m_radius = m_InitialRadius;
	}

	if (m_play)
	{
		m_position += m_velocity * delta_time;
	}
}


