#include "btpch.h"
#include "PhysXRigidbody.h"

namespace Becketron {

	PhysXRigidbody::PhysXRigidbody(PhysXScene* scene, glm::vec3 pos, glm::quat rot) : m_Scene(scene)
	{
		physx::PxVec3 PxPos(pos.x, pos.y, pos.z);
		physx::PxQuat PxQua(rot.x, rot.y, rot.z, rot.w);

		physx::PxTransform Transform(PxPos, PxQua);

		m_Body = m_Scene->m_Physics->createRigidDynamic(Transform);
		SetKinematic(false);
	}

	PhysXRigidbody::~PhysXRigidbody()
	{
		m_Scene->m_Physics->release();
	}

	void PhysXRigidbody::SetKinematic(bool kinematic)
	{
		if (kinematic)
		{
			m_Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
			m_Kinematic = true;
		}
		else if (!kinematic)
		{
			m_Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
			m_Kinematic = false;
		}

	}

	bool PhysXRigidbody::IsKinematic()
	{
		return m_Kinematic;
	}
}