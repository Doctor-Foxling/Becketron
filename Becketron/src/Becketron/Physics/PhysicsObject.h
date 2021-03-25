#pragma once

#include "glm/vec3.hpp"
#include "Becketron/Physics/BoundingSphere.h"

class PhysicsObject
{
public:
	PhysicsObject(glm::vec3&& position, glm::vec3&& velocity, float radius) :
		m_position(position),
		m_velocity(velocity),
		m_radius(radius) {}

	PhysicsObject(glm::vec3& position, glm::vec3& velocity, float radius) :
		m_position(position),
		m_velocity(velocity),
		m_radius(radius) {}

	void Integrate(float delta_time);

	const glm::vec3& GetPosition() const { return m_position; }
	const glm::vec3& GetVelocity() const { return m_velocity; }

	float GetRadius() const { return m_radius; }

	// Create boundingSphere on the fly when requested
	BoundingSphere GetBoundingSphere() const
	{
		return BoundingSphere(m_position, m_radius);
	}
private:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	float	  m_radius;
};