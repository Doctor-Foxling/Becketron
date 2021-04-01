#pragma once

#include "glm/vec3.hpp"
#include "Becketron/Physics/BoundingSphere.h"

class PhysicsObject
{
public:
	PhysicsObject(glm::vec3&& position, glm::vec3&& velocity, float radius) :
		m_position(position), m_InitialPosition(position),
		m_velocity(velocity), m_InitialVelocity(velocity),
		m_radius(radius), m_InitialRadius(radius) {}

	PhysicsObject(glm::vec3& position, glm::vec3& velocity, float radius) :
		m_position(position), m_InitialPosition(position),
		m_velocity(velocity), m_InitialVelocity(velocity),
		m_radius(radius), m_InitialRadius(radius) {}

	void Integrate(float delta_time);

	const glm::vec3& GetPosition() const { return m_position; }
	const glm::vec3& GetVelocity() const { return m_velocity; }

	void SetVelocity(const glm::vec3& velocity) { m_velocity = velocity; }

	float GetRadius() const { return m_radius; }
	bool	  m_play = false;

	// Create boundingSphere on the fly when requested
	BoundingSphere GetBoundingSphere() const
	{
		return BoundingSphere(m_position, m_radius);
	}
private:
	glm::vec3 m_InitialPosition;
	glm::vec3 m_position;
	glm::vec3 m_InitialVelocity;
	glm::vec3 m_velocity;
	float	  m_InitialRadius;
	float	  m_radius;
	bool	  m_playLast = false;
};