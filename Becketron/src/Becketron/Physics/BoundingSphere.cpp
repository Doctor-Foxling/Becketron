#include "btpch.h"

#include "BoundingSphere.h"

IntersectData BoundingSphere::IntersectBoundingSphere(const BoundingSphere& other)
{
	float radiusDistance = m_radius + other.m_radius;
	float centerDistance = glm::length(other.GetCenter() - m_center);

	glm::vec3 centerVecDist = other.GetCenter() - m_center;
	BT_TRACE("other center = {0}, {1}, {2}, my center = {3}, {4}, {5}, the length = {6}", other.GetCenter().x,
		other.GetCenter().y, other.GetCenter().z, m_center.x, m_center.y, m_center.z, centerDistance);
	BT_TRACE("Radius = {0}, other Radius = {1}, centerVecDist = [ {2}, {3}, {4}]", m_radius, other.m_radius,
		centerVecDist.x, centerVecDist.y, centerVecDist.z)
	BT_TRACE("Center - Radius = {0}", centerDistance - radiusDistance);

	if (centerDistance < radiusDistance)
	{
		return IntersectData(true, centerDistance - radiusDistance);
	}
	else
	{
		return IntersectData(false, centerDistance - radiusDistance);
	}
}

void BoundingSphere::Test()
{
	BoundingSphere sphere1(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	BoundingSphere sphere2(glm::vec3(0.0f, 3.0f, 0.0f), 1.0f);
	BoundingSphere sphere3(glm::vec3(0.0f, 0.0f, 2.0f), 1.0f);
	BoundingSphere sphere4(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);

	IntersectData sphere1IntersectSphere2 = sphere1.IntersectBoundingSphere(sphere2);
	IntersectData sphere1IntersectSphere3 = sphere1.IntersectBoundingSphere(sphere3);
	IntersectData sphere1IntersectSphere4 = sphere1.IntersectBoundingSphere(sphere4);

	BT_INFO("Sphere 1 Intertersect Sphere 2 result: {0}", sphere1IntersectSphere2.GetDoesIntersect());
	BT_INFO("Sphere 1 Intertersect Sphere 3 result: {0}", sphere1IntersectSphere3.GetDoesIntersect());
	BT_INFO("Sphere 1 Intertersect Sphere 4 result: {0}", sphere1IntersectSphere4.GetDoesIntersect());
}