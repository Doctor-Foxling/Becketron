#pragma once

#include "glm/vec3.hpp"
#include "Becketron/Physics/BoundingSphere.h"
#include "Becketron/Physics/aabb.h"

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

	// AABB
	PhysicsObject(glm::vec3&& position, glm::vec3&& velocity, glm::vec3 scale) :
		m_position(position), m_InitialPosition(position),
		m_velocity(velocity), m_InitialVelocity(velocity),
		m_scale(scale), m_InitialScale(scale) {}

	void Integrate(float delta_time);

	const glm::vec3& GetPosition() const { return m_position; }
	const glm::vec3& GetVelocity() const { return m_velocity; }

	void SetPosition(const glm::vec3& pos) { m_position = pos; }
	void SetVelocity(const glm::vec3& velocity) { m_velocity = velocity; }

	// For Bouding Sphere
	float GetRadius() const { return m_radius; }

	// For AABB (Axis Aligned Bouding Boxes)
	glm::vec3 GetScale() const { return m_scale; }
	void SetScale(const glm::vec3& scale) { m_scale = scale; }

	bool m_play = false;
	bool m_restart = false;

	// Create boundingSphere on the fly when requested
	BoundingSphere GetBoundingSphere() const
	{
		return BoundingSphere(m_position, m_radius);
	}

	AABB GetAABB() const
	{
		return AABB(m_position - (m_scale/glm::vec3(2.0f)), m_position + (m_scale/glm::vec3(2.0f)));
	}

private:
	glm::vec3 m_InitialPosition;
	glm::vec3 m_position;
	glm::vec3 m_InitialVelocity;
	glm::vec3 m_velocity;

	// Bounding sphere
	float	  m_InitialRadius;
	float	  m_radius = 0;

	// AABB
	glm::vec3 m_scale = glm::vec3(0);
	glm::vec3 m_InitialScale;

	bool	  m_lastRestart = false;
};