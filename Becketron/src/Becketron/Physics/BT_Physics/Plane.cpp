#include "btpch.h"

#include "plane.h"

Plane Plane::Normalized() const
{
	float magnitude = m_normal.length();

	return Plane(m_normal / magnitude, m_distance / magnitude);
}

IntersectData Plane::IntersectSphere(const BoundingSphere& other) const
{;
	float distanceFromSphereCenter = fabs(glm::dot(m_normal, other.GetCenter()) + m_distance);
	float distanceFromSphere = distanceFromSphereCenter - other.GetRadius();

	return IntersectData(distanceFromSphere < 0, distanceFromSphere);
}