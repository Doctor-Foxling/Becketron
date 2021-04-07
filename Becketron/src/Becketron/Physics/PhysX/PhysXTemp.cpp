#include "btpch.h"
#include "PhysXTemp.h"

namespace Becketron
{
	/*
	static 

	physx::PxRigidDynamic* PhysXTemp::createDynamic(const physx::PxTransform& t,
		const physx::PxGeometry& geometry, const physx::PxVec3& velocity = physx::PxVec3(0))
	{
		physx::PxRigidDynamic* dynamic = physx::PxCreateDynamic(*s_PXPhysicsFactory, t, geometry, *gMaterial, 10.0f);
		dynamic->setAngularDamping(0.5f);
		dynamic->setLinearVelocity(velocity);
		m_PxScene->addActor(*dynamic);
		return dynamic;
	}

	static physx::PxTransform glmToPhysxTransform(const glm::mat4& Transform)
	{
		physx::PxVec3 PxPos;
		const glm::mat4& transform = Transform;
		auto [pos, rot, sacle] = GetTransformDecomposition(transform);
		PxPos.x = pos.x;
		PxPos.y = pos.y;
		PxPos.z = pos.z;

		physx::PxQuat PxQua;
		PxQua.x = rot.x;
		PxQua.y = rot.y;
		PxQua.z = rot.z;
		PxQua.w = rot.w;

		physx::PxTransform PhysXTransform(PxPos, PxQua);
	}

	physx::PxRigidDynamic* CreateRigidDynamic(physx::PxTransform trans)
	{
		physx::PxRigidDynamic* p_body = s_PXPhysicsFactory->createRigidDynamic(trans);
		return p_body;
	}
	*/
}