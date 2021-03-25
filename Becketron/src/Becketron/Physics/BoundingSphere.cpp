#include "btpch.h"

#include "BoundingSphere.h"

IntersectData BoundingSphere::IntersectBoundingSphere(const BoundingSphere& other)
{
	float radiusDistance = m_radius + other.m_radius;
	float centerDistance = (other.GetCenter() - m_center).length();

	if (centerDistance < radiusDistance)
	{
		return IntersectData(true, centerDistance - radiusDistance);
	}
	else
	{
		return IntersectData(false, centerDistance - radiusDistance);
	}
}