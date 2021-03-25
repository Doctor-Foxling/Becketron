#include "btpch.h"

#include "aabb.h"

template <typename T>
T Max(glm::vec3& vect)
{
	T maxVal = vect.x;

	if (vect.y > maxVal)
		maxVal = vect.y;
	if (vect.z > maxVal)
		maxVal = vect.z;

	return maxVal;
}

IntersectData AABB::IntersectAABB(const AABB& other) const
{
	glm::vec3 distances1 = other.GetMinExtents() - m_maxExtents;
	glm::vec3 distances2 = m_minExtents - other.GetMaxExtents();
	glm::vec3 distances = glm::max(distances1, distances2);

	float maxDistance = Max<float>(distances);

	return IntersectData(maxDistance < 0, maxDistance);
}

void AABB::Test()
{
	// Random testing
	AABB aabb1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	AABB aabb2(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	AABB aabb3(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	AABB aabb4(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(1.0f, 1.0f, -1.0f));
	AABB aabb5(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f, 1.5f, 1.0f));

	IntersectData aabb1Intersectaabb2 = aabb1.IntersectAABB(aabb2);
	IntersectData aabb1Intersectaabb3 = aabb1.IntersectAABB(aabb3);
	IntersectData aabb1Intersectaabb4 = aabb1.IntersectAABB(aabb4);
	IntersectData aabb1Intersectaabb5 = aabb1.IntersectAABB(aabb5);

	BT_CORE_INFO("AABB1 intersect AABB2: {0}, Distance: {1}", aabb1Intersectaabb2.GetDoesIntersect(),
		aabb1Intersectaabb2.GetDistance());

	BT_CORE_INFO("AABB1 intersect AABB3: {0}, Distance: {1}", aabb1Intersectaabb3.GetDoesIntersect(),
		aabb1Intersectaabb3.GetDistance());

	BT_CORE_INFO("AABB1 intersect AABB4: {0}, Distance: {1}", aabb1Intersectaabb4.GetDoesIntersect(),
		aabb1Intersectaabb4.GetDistance());

	BT_CORE_INFO("AABB1 intersect AABB5: {0}, Distance: {1}", aabb1Intersectaabb5.GetDoesIntersect(),
		aabb1Intersectaabb5.GetDistance());
}