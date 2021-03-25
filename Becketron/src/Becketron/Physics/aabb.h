#pragma once

#include "glm/glm.hpp"
#include "IntersectData.h"

class AABB
{
public:
	AABB(const glm::vec3& minExtents, const glm::vec3& maxExtents) :
		m_minExtents(minExtents), m_maxExtents(maxExtents) {}

	IntersectData IntersectAABB(const AABB& other) const;

	const glm::vec3& GetMinExtents() const { return m_minExtents; }
	const glm::vec3& GetMaxExtents() const { return m_maxExtents; }

	void Test();
private:
	const glm::vec3 m_minExtents;
	const glm::vec3 m_maxExtents;
};