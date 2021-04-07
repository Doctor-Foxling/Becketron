#pragma once

#include <PhysX/PxPhysicsAPI.h>


namespace Becketron
{
	class PhysXTemp
	{
	public:
		static physx::PxRigidDynamic* createDynamic(const physx::PxTransform& t,
			const physx::PxGeometry& geometry, const physx::PxVec3& velocity = physx::PxVec3(0));
	private:
		physx::PxMaterial* m_Material = NULL;
	};
}