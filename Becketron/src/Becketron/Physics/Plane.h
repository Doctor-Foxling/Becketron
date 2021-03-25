#pragma once

#include <glm/vec3.hpp>

#include "Becketron/Physics/BoundingSphere.h"

class Plane
{
public:
	Plane(const glm::vec3& normal, float distance) :
		m_normal(normal),
		m_distance(distance) {}

	// Because our m_normal isn't necessarily normalized, we give the option to turn it into unit length
	Plane Normalized() const;
	IntersectData IntersectSphere(const BoundingSphere& other) const;

	const glm::vec3& GetNormal() const { return m_normal; }
	float GetDistance()			 const { return m_distance; }
private:
	const glm::vec3 m_normal;
	// m_distance is how far the plane itself is along its normal
	const float m_distance;
};