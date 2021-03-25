#pragma once

#include <glm/glm.hpp>
#include "IntersectData.h"

class BoundingSphere
{
public:
	BoundingSphere(const glm::vec3& center, float radius) :
		m_center(center),
		m_radius(radius) {}

	IntersectData IntersectBoundingSphere(const BoundingSphere& other);

	const glm::vec3& GetCenter() const { return m_center; }
	float GetRadius()			 const { return m_radius; }
private:
	const glm::vec3& m_center;
	const float		 m_radius;
};