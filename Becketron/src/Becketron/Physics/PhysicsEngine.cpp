#include "btpch.h"

#include "PhysicsEngine.h"

void PhysicsEngine::AddObject(PhysicsObject* object)
{
	m_objects.push_back(object);
}

void PhysicsEngine::Simulate(float delta_time)
{
	for (unsigned int i = 0; i < m_objects.size(); i++)
	{
		m_objects[i]->Integrate(delta_time);
	}
}

void PhysicsEngine::HandleCollision(CollisionType type)
{
	for (unsigned int i = 0; i < m_objects.size(); i++)
	{
		for (unsigned int j = i + 1; j < m_objects.size(); j++)
		{
			if (type == CollisionType::BoundingSphere)
			{
				IntersectData intersectData =
					m_objects[i]->GetBoundingSphere().IntersectBoundingSphere(
						m_objects[j]->GetBoundingSphere());

				if (intersectData.GetDoesIntersect())
				{
					BT_CORE_INFO("Object {0} collided with Object {1}", i, j);
					m_objects[i]->SetVelocity(m_objects[i]->GetVelocity() * -1.0f);
					m_objects[j]->SetVelocity(m_objects[j]->GetVelocity() * -1.0f);
				}
			}

			if (type == CollisionType::AABB)
			{
				IntersectData intersectData =
					m_objects[i]->GetAABB().IntersectAABB(m_objects[j]->GetAABB());

				if (intersectData.GetDoesIntersect())
				{
					BT_CORE_INFO("Object {0} collided with Object {1}", i, j);
					m_objects[i]->SetVelocity(m_objects[i]->GetVelocity() * -1.0f);
					m_objects[j]->SetVelocity(m_objects[j]->GetVelocity() * -1.0f);
				}
			}

			 

			
		}
	}
}